#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#ifdef WINDOWS

#define CLOSE_SOCKET closesocket
#define ssize_t int
#define socklen_t int

#include "winsock2.h"

#else

#define INVALID_SOCKET 0
#define SOCKET_ERROR -1
#define CLOSE_SOCKET close

#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "unistd.h"
#include <cstdio>

typedef int SOCKET;

#endif

#include "../type/String.hpp"
#include "../core/Core.hpp"
#include "../core/HttpResponse.hpp"
#include "../core/HttpRequest.hpp"

#include <thread>
#include <queue>

struct IncomingConnection {
    struct sockaddr_in client_address{};
    socklen_t client_length = sizeof(client_address);
    SOCKET incoming_socket = INVALID_SOCKET;
};

class HttpServer {
public:
    explicit HttpServer(size_t port = 80);

    virtual ~HttpServer();

    int eventLoop();

    static String process(const HttpRequest &request);

protected:
#ifdef WINDOWS
    WSAData wsa_data_;
#endif

    enum class Status {
        Ok = 0,
        UnknownError,
        UnableToOpenSocket,
    };

    unsigned free_connection_slots_;
    unsigned long long request_count_, response_count_;
    size_t port_;
    std::queue<IncomingConnection> connections_queue_;
    SOCKET server_socket_;
    sockaddr_in server_address_;

    bool createSocket();

    void destroySocket();

    void run();

    void handle(IncomingConnection &conn);

private:
    int announce_rate_;

    void putCurrentThreadToSleep() const;
};

#endif // HTTPSERVER_H
