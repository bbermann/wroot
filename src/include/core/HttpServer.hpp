#pragma once

#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/ts/internet.hpp>
#include <include/network/Connection.hpp>
#include <include/network/ConnectionManager.hpp>
#include <include/network/http/RequestHandler.hpp>
#include <include/type/String.hpp>

class HttpServer {
public:
    HttpServer(const HttpServer&) = delete;
    HttpServer& operator=(const HttpServer&) = delete;

    explicit HttpServer(const String &address, size_t port);

    void run();

private:
    void accept();

    void awaitStop();

    /// The io_service used to perform asynchronous operations.
    boost::asio::io_service ioService_;

    /// The signal_set is used to register for process termination notifications.
    boost::asio::signal_set signals_;

    /// Acceptor used to listen for incoming connections.
    boost::asio::ip::tcp::acceptor acceptor_;

    /// The next socket to be accepted.
    boost::asio::ip::tcp::socket socket_;

    /// The connection manager which owns all live connections.
    ConnectionManager connectionManager_;

    /// The handler for all incoming requests.
    RequestHandler requestHandler_;
};
