#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#define KEEP_ALIVE_MS 5000

#ifdef WINDOWS
#define CLOSESOCKET closesocket

#include "winsock2.h"

#define ssize_t int
#define socklen_t int
#else
#define INVALID_SOCKET 0
#define SOCKET_ERROR -1
#define CLOSESOCKET close

#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "stdio.h"

typedef int SOCKET;
#endif

#include "../type/String.hpp"
#include "../core/Core.hpp"
#include "../core/HttpResponse.hpp"
#include "../core/HttpRequest.hpp"

#include <thread>
#include <queue>

struct IncommingConnection {
    struct sockaddr_in client_address;
    socklen_t client_length = sizeof(client_address);
    SOCKET incomming_socket = INVALID_SOCKET;
};

class HttpServer {
public:
    HttpServer(size_t port = 80);

    virtual ~HttpServer();

    void start();

    ///<summary>
    ///Executa o loop principal de interpretação de url e retorno.
    ///</summary>
    ///<example>
    ///ExecAPI("/estoque/produto/7893546218542/", "GET");
    ///</example>
    static String process(const HttpRequest &request);

protected:
#ifdef WINDOWS
    WSAData wsa_data_;
#endif

    enum HeaderParameter {
        ConnectionKeepAlive,
        ConnectionClose,
    };

    unsigned free_connection_slots_, request_count_, response_count_;
    size_t port_;
    std::queue <IncommingConnection> client_pending_;
    SOCKET server_socket_;
    sockaddr_in server_address_;

    bool createSocket();

    void destroySocket();

    void getUrl(String &url, String &library, String &function, StringList &arguments);

    void run();

    void handle(IncommingConnection &conn);

    String getHttpHeader(String request);

    bool getHeaderParameter(String header, HeaderParameter parameter);

private:
    int announce_rate_;
};

#endif // HTTPSERVER_H
