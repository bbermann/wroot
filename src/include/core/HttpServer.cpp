#include "HttpServer.hpp"
#include "Core.hpp"
#include <signal.h>
#include <utility>
#include <thread>
#include <functional>
#include <memory>

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

    using asio::ip::tcp;

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

//    // TODO: Implement Http Server 3 example (
//    //  https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio/examples/cpp03_examples.html
//    // )
//    // Pendente coroutines (migrar p/ clang ou aguardar implementação do g++)
//
//    // For the sake of simplicity...
//    using namespace std;
//
//    // Create a pool of threads to run all of the io_services.
//    vector<shared_ptr<thread>> threads;
//
//    for (size_t i = 0; i < Core::ThreadCount; ++i) {
//        auto threadPtr = make_shared<thread>([this]() {
//            this->ioService_.run();
//        });
//
//        threads.push_back(threadPtr);
//    }
//
//    // Wait for all threads in the pool to exit.
//    for (auto &thread : threads) {
//        thread->join();
//    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "InfiniteRecursion"

void HttpServer::accept() {
    this->acceptor_.async_accept(
            socket_,
            [this](asio::error_code ec) {
                // Check whether the server was stopped by a signal before this
                // completion handler had a chance to run.
                if (!this->acceptor_.is_open()) {
                    return;
                }

                if (!ec) {
                    this->connectionManager_.start(
                            std::make_shared<Connection>(
                                    std::move(this->socket_),
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
            [this](asio::error_code /*ec*/, int /*signo*/) {
                // The server is stopped by cancelling all outstanding asynchronous
                // operations. Once all operations have finished the io_service::run()
                // call will exit.
                this->acceptor_.close();

                this->connectionManager_.stopAll();
            }
    );
}