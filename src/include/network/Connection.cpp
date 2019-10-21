//
// Created by bbermann on 10/10/2019.
//

#include "Connection.hpp"
#include "ConnectionManager.hpp"
#include "include/network/http/RequestHandler.hpp"
#include "http/Response.hpp"
#include <utility>
#include <vector>

Connection::Connection(asio::ip::tcp::socket socket, ConnectionManager &manager, RequestHandler &handler)
        : socket_(std::move(socket)),
          connectionManager_(manager),
          requestHandler_(handler) {
    this->socket_.set_option(asio::ip::tcp::no_delay(true));
}

void Connection::start() {
    this->read();
}

void Connection::stop() {
    this->socket_.close();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "InfiniteRecursion"
void Connection::read() {
    auto self(this->shared_from_this());

    this->socket_.async_read_some(
            asio::buffer(this->buffer_),
            [this, self](asio::error_code error, std::size_t bytes_transferred) {
                if (!error) {
                    RequestParser::ResultType result;

                    std::tie(result, std::ignore) = requestParser_.parse(
                            request_,
                            buffer_.data(),
                            buffer_.data() + bytes_transferred
                    );

                    if (result == RequestParser::Good) {
                        requestHandler_.handle(this->request_, this->response_);

                        this->write();
                    } else if (result == RequestParser::Bad) {
                        this->response_ = Response::stockResponse(Response::BadRequest);

                        this->write();
                    } else {
                        this->read();
                    }
                } else if (error != asio::error::operation_aborted) {
                    this->connectionManager_.stop(this->shared_from_this());
                }
            }
    );
}
#pragma clang diagnostic pop

void Connection::write() {
    auto self(this->shared_from_this());

    asio::async_write(
            this->socket_,
            this->response_.toBuffers(),
            [this, self](asio::error_code ec, std::size_t) {
                if (!ec) {
                    // Initiate graceful connection closure.
                    asio::error_code ignored_ec;

                    this->socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ignored_ec);
                }

                if (ec != asio::error::operation_aborted) {
                    this->connectionManager_.stop(this->shared_from_this());
                }
            }
    );
}