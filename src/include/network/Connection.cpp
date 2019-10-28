//
// Created by bbermann on 10/10/2019.
//

#include "Connection.hpp"
#include "ConnectionManager.hpp"
#include <boost/system/error_code.hpp>
#include <include/network/http/RequestHandler.hpp>
#include <utility>

Connection::Connection(boost::asio::ip::tcp::socket socket, ConnectionManager &manager, RequestHandler &handler)
        : socket_(std::move(socket)),
          connectionManager_(manager),
          requestHandler_(handler) {
    this->socket_.set_option(boost::asio::ip::tcp::no_delay(true));

    Core::debugLn(
            "Connected to " +
            this->socket_.remote_endpoint().address().to_string() + ":" + // Remote Address
            std::to_string(socket_.remote_endpoint().port()) // Remote Port
    );
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
            boost::asio::buffer(this->buffer_),
            [this, self](boost::system::error_code error, std::size_t bytes_transferred) {
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
                } else if (error != boost::asio::error::operation_aborted) {
                    this->connectionManager_.stop(this->shared_from_this());
                }
            }
    );
}

#pragma clang diagnostic pop

void Connection::write() {
    auto self(this->shared_from_this());

    boost::asio::async_write(
            this->socket_,
            this->response_.toBuffers(),
            [this, self](boost::system::error_code ec, std::size_t) {
                if (!ec) {
                    // Initiate graceful connection closure.
                    boost::system::error_code ignored_ec;

                    this->socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
                }

                if (ec != boost::asio::error::operation_aborted) {
                    this->connectionManager_.stop(this->shared_from_this());
                }
            }
    );
}