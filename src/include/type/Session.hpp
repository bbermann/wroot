//
// Created by bbermann on 09/10/2019.
//

#pragma once

#include <include/core/Core.hpp>
#include <include/network/http/Response.hpp>
#include <include/network/http/Request.hpp>
#include <include/exceptions/http/request/FailedReceivingData.hpp>
#include <include/type/String.hpp>

#include <asio/ts/internet.hpp>
#include <cstdio>
#include <optional>
#include <queue>
#include <thread>
#include <memory>
#include <stddef.h>
#include <utility>

struct IncomingConnection {
    IncomingConnection(asio::ip::tcp::socket socket, const Request &request)
            : socket(std::move(socket)),
              request(request) {
    }

    asio::ip::tcp::socket socket;
    Request request;
};

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(asio::ip::tcp::socket socket) : socket_(std::move(socket)) {
    }

    void start() {
        this->read();
    }

    void handle(const IncomingConnection &conn) {
        String response = Session::process(conn.request);

        this->sendResponse(conn, response);
    }

    Request receiveRequest(const IncomingConnection &conn) {
        char recvBuffer[Core::kBufferSize];

        //Retrieve the client ip address and announce it when debugging
        std::string clientIpAddress = conn.socket.remote_endpoint().address().to_string();
        Core::debugLn("Handling request from " + clientIpAddress + "...");

        String receivedData = "";

        do {
            auto bytesReceived = recv(conn.socket, recvBuffer, Core::kBufferSize, 0);

            // Client disconnect or error receiving request
            if (bytesReceived <= 0) {
                throw FailedReceivingData();
            }

            // The request should not be fully received yet, let's read some more data
            if (bytesReceived == Core::kBufferSize) {
                retry = true;
            }

            // Store the current buffered data to join request parts
            receivedData.append(recvBuffer);
        } while (retry);

        return Request(receivedData, clientIpAddress);
    }

    void sendResponse(const IncomingConnection &conn, const String &response) {
        if (response.length() > 0) {
            ssize_t bytesSent;

            do {
                bytesSent = send(conn.socket, response.data(), response.length(), MSG_NOSIGNAL);
            } while (errno == EWOULDBLOCK);

            Core::debugLn("Bytes Sent: " + std::to_string(bytesSent));
        }

        Core::ThreadMutex.lock();

        ++response_count_;

        if (response_count_ % this->announce_rate_ == 0) {
            Core::debugLn("Response count: " + std::to_string(response_count_));
        }

        Core::ThreadMutex.unlock();
    }

    static String process(const Request &request) {
        auto responseHandler = std::async(std::launch::async, [request] {
            try {
                std::shared_ptr<CustomLibrary> app(new FileLibrary(request));

                Response response = app->getResponse();

                if (response.status == 404) {
                    app.reset(new RouterLibrary(request));

                    response = std::move(app->getResponse());
                }

                return response.toString();
            }
            catch (const std::runtime_error &ex) {
                Core::warning(String("Runtime error silenced by HttpServer: ") + ex.what(), "HttpServer::process");
            }
            catch (const std::exception &ex) {
                Core::warning(String("Exception silenced by HttpServer: ") + ex.what(), "HttpServer::process");
            }

            return Response(500).toString();
        });

        auto responseStatus = responseHandler.wait_for(
                std::chrono::milliseconds(Core::RequestTimeout)
        );

        if (responseStatus == std::future_status::timeout) {
            throw RequestTimeoutException();
        }

        return responseHandler.get();
    }

private:
    void read() {
        auto self(this->shared_from_this());

        this->socket_.async_read_some(
                asio::buffer(this->data_, this->MaxLength),
                [this, self](std::error_code errorCode, std::size_t length) {
                    if (errorCode) {
                        Core::debugLn(String("Silent error code: ") + errorCode.message());
                        return;
                    };

                    try {
                        IncomingConnection conn(std::move(this->socket_), this->receiveRequest(conn));

                        this->write(conn);
                    }
                    catch (const FailedReceivingData &exception) {
                        Core::debugLn(String("Silent exception: ") + exception.what());
                    }
                }
        );
    }

    void write(const IncomingConnection &conn) {
        auto self(this->shared_from_this());

        asio::async_write(
                this->socket_,
                asio::buffer(this->data_, length),
                [this, self, &conn](std::error_code errorCode, std::size_t /*length*/) {
                    if (errorCode) {
                        Core::debugLn(String("Silent error code: ") + errorCode.message());
                    }

                    try {
                        this->handle(conn);
                    }
                    catch (const std::exception &exception) {
                        Core::debugLn(String("Silent exception: ") + exception.what());
                    }
                }
        );
    }

    asio::ip::tcp::socket socket_;
    enum {
        MaxLength = 1024
    };
    char data_[MaxLength];
};
