//
// Created by bbermann on 10/10/2019.
//

#pragma once

#include <array>
#include <memory>
#include <boost/asio.hpp>
#include <include/core/Core.hpp>
#include <include/network/http/Response.hpp>
#include <include/network/http/Request.hpp>
#include <include/network/http/RequestHandler.hpp>
#include <include/network/http/RequestParser.hpp>

class ConnectionManager;

/// Represents a single connection from a client.s
class Connection : public std::enable_shared_from_this<Connection> {
public:
    Connection(const Connection &) = delete;

    Connection &operator=(const Connection &) = delete;

    /// Construct a connection with the given socket.
    explicit Connection(asio::ip::tcp::socket socket, ConnectionManager &manager, RequestHandler &handler);

    /// Start the first asynchronous operation for the connection.
    void start();

    /// Stop all asynchronous operations associated with the connection.
    void stop();

private:
    /// Perform an asynchronous read operation.
    void read();

    /// Perform an asynchronous write operation.
    void write();

    /// Socket for the connection.
    asio::ip::tcp::socket socket_;

    /// The manager for this connection.
    ConnectionManager &connectionManager_;

    /// The handler used to process the incoming request.
    RequestHandler &requestHandler_;

    /// Buffer for incoming data.
    std::array<char, Core::BufferSize> buffer_;

    /// The incoming request.
    Request request_;

    /// The parser for the incoming request.
    RequestParser requestParser_;

    /// The reply to be sent back to the client.
    Response response_;
};

typedef std::shared_ptr<Connection> ConnectionPtr;