//
// Created by bbermann on 10/10/2019.
//

#include "ConnectionManager.hpp"

ConnectionManager::ConnectionManager() {
}

void ConnectionManager::start(ConnectionPtr connection) {
    this->connections_.insert(connection);

    connection->start();
}

void ConnectionManager::stop(ConnectionPtr connection) {
    this->connections_.erase(connection);

    connection->stop();
}

void ConnectionManager::stopAll() {
    for (auto connection : this->connections_) {
        connection->stop();
    }

    this->connections_.clear();
}