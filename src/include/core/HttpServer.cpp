#include "HttpServer.hpp"
#include <signal.h>
#include <utility>
#include <memory>
#include <boost/system/error_code.hpp>

using boost::asio::ip::tcp;

HttpServer::HttpServer(const String &address, size_t port)
        : ioService_(),
          signals_(ioService_),
          acceptor_(ioService_),
          connectionManager_(),
          socket_(ioService_),
          requestHandler_() {
    // Register to handle the signals that indicate when the server should exit.
    // It is safe to register for the same signal multiple times in a program,
    // provided all registration for the specified signal is made through Asio.
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
#if defined(SIGQUIT)
    signals_.add(SIGQUIT);
#endif

    this->awaitStop();

    tcp::resolver resolver(this->ioService_);
    tcp::endpoint endpoint = *resolver.resolve({address, std::to_string(port)});

    this->acceptor_.open(endpoint.protocol());
    this->acceptor_.set_option(tcp::acceptor::reuse_address(true));
    this->acceptor_.set_option(tcp::acceptor::keep_alive(true));
    this->acceptor_.bind(endpoint);
    this->acceptor_.listen();

    this->accept();
}

void HttpServer::run() {
    // The io_service::run() call will block until all asynchronous operations
    // have finished. While the server is running, there is always at least one
    // asynchronous operation outstanding: the asynchronous accept call waiting
    // for new incoming connections.
    this->ioService_.run();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "InfiniteRecursion"

void HttpServer::accept() {
    auto socket = std::make_shared<tcp::socket>(ioService_);

    this->acceptor_.async_accept(
            *socket,
            [this, socket](boost::system::error_code errorCode) {
                // Check whether the server was stopped by a signal before this
                // completion handler had a chance to run.
                if (!this->acceptor_.is_open()) {
                    return;
                }

                if (!errorCode) {
                    this->connectionManager_.start(
                            std::make_shared<Connection>(
                                    std::move(*socket),
                                    this->connectionManager_,
                                    this->requestHandler_
                            )
                    );
                }

                this->accept();
            }
    );
}

#pragma clang diagnostic pop

void HttpServer::awaitStop() {
    this->signals_.async_wait(
            [this](boost::system::error_code /*ec*/, int /*signo*/) {
                // The server is stopped by cancelling all outstanding asynchronous
                // operations. Once all operations have finished the io_service::run()
                // call will exit.
                this->acceptor_.close();

                this->connectionManager_.stopAll();
            }
    );
}