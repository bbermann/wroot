//
// Created by bbermann on 10/10/2019.
//

#pragma once

#include <set>
#include "Connection.hpp"

/// Manages open connections so that they may be cleanly stopped
/// when the server needs to shut down.
class ConnectionManager {
public:
    ConnectionManager(const ConnectionManager &) = delete;
    ConnectionManager &operator=(const ConnectionManager &) = delete;

    /// Construct a connection manager.
    ConnectionManager();

    /// Add the specified connection to the manager and start it.
    void start(ConnectionPtr c);

    /// Stop the specified connection.
    void stop(ConnectionPtr c);

    /// Stop all connections.
    void stopAll();

private:
    /// The managed connections.
    std::set<ConnectionPtr> connections_;
};
