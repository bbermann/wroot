#include <include/core/HttpServer.hpp>
#include <include/library/FileLibrary.hpp>
#include <include/library/RouterLibrary.hpp>
#include <include/exceptions/http/request/FailedReceivingData.hpp>
#include <exception>
#include <future>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#ifdef WINDOWS
#undef errno
#define errno WSAGetLastError()
#ifndef EWOULDBLOCK
#define EWOULDBLOCK WSAEWOULDBLOCK
#endif
#else

#include <fcntl.h>  /* For O_RDWR */
#include <unistd.h> /* For open(), creat() */

#endif

HttpServer::HttpServer(size_t port) {
    this->port_ = port;
    this->server_socket_ = INVALID_SOCKET;

    if (!this->createSocket())
        exit(2);

    this->free_connection_slots_ = Core::kMaxConnections;
    this->announce_rate_ = Core::IsDebugging ? 10 : 1000;
}

HttpServer::~HttpServer() {
    this->destroySocket();
}

bool HttpServer::createSocket() {
    bool bReturn = false;

#ifdef WINDOWS
    if (WSAStartup(MAKEWORD(Core::kWSockVersion, 0), &wsa_data_) == NO_ERROR)
    {
        if (LOBYTE(wsa_data_.wVersion) >= Core::kWSockVersion)
        {
            server_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (server_socket_ == INVALID_SOCKET)
            {
                Core::error("Create socket failed: " + WSAGetLastError());
            }
            else
            {
                bReturn = true;
            }
        }
        else
        {
            Core::error("WinSocket version is lower than " + Core::kWSockVersion, "bool HttpServer::createSocket()");
        }
    }

    if (!bReturn)
    {
        Core::error("WSAStartup has failed");
        WSACleanup();
    }
    else
    {
        server_address_.sin_family = AF_INET;
        server_address_.sin_addr.s_addr = INADDR_ANY;
        server_address_.sin_port = htons(port_);

        const char *reuseSocket = "1";
        setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, reuseSocket, sizeof(reuseSocket));
        //setsockopt(server_socket_, SOL_SOCKET, SO_KEEPALIVE, reuseSocket, sizeof(reuseSocket));

        // If iMode!=0, non-blocking mode is enabled.
        u_long iMode = 1;
        ioctlsocket(server_socket_, FIONBIO, &iMode);

        if (::bind(server_socket_, (SOCKADDR *)&server_address_, sizeof(server_address_)) == SOCKET_ERROR)
        {
            Core::error("Socket binding has failed.");
            CLOSE_SOCKET(server_socket_);
            bReturn = false;
        }
    }
#else
    server_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_socket_ == INVALID_SOCKET) {
        Core::error("Create socket failed!");
    } else {
        server_address_.sin_family = AF_INET;
        server_address_.sin_addr.s_addr = INADDR_ANY;
        server_address_.sin_port = htons(port_);

        int reuseSocket = 1;
        setsockopt(server_socket_, SOL_SOCKET, SO_REUSEPORT, &reuseSocket, sizeof(reuseSocket));
        setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &reuseSocket, sizeof(reuseSocket));
        setsockopt(server_socket_, SOL_SOCKET, SO_KEEPALIVE, &reuseSocket, sizeof(reuseSocket));

#ifdef WROOT_NONBLOCKING_IO
        int fcntl_status = fcntl(server_socket_, F_SETFL, fcntl(server_socket_, F_GETFL, 0) | O_NONBLOCK);
#else
        int fcntl_status = fcntl(server_socket_, F_SETFL, fcntl(server_socket_, F_GETFL, 0));
#endif

        if (fcntl_status == -1) {
            Core::error("Failed to set non blocking option on the server socket.", "bool HttpServer::createSocket()");
        }

        if (bind(server_socket_, (struct sockaddr *) &server_address_, sizeof(server_address_)) == SOCKET_ERROR) {
            Core::error("Socket binding has failed.");
            CLOSE_SOCKET(server_socket_);
            bReturn = false;
        } else {
            bReturn = true;
        }
    }
#endif

    return bReturn;
}

int HttpServer::eventLoop() {
    if (listen(server_socket_, Core::kMaxConnections) == SOCKET_ERROR) {
        Core::error("Unable to open socket on port " + std::to_string(port_) + ".");
        return (int) Status::UnableToOpenSocket;
    }

    request_count_ = 0;
    response_count_ = 0;

    //Uma thread (std::this_thread) é reservada para escutar a porta de entrada.
    const unsigned int listenThreadsCount = 1;
    //Ao menos uma thread é necessária para processar as respostas...
    unsigned int workerThreadsCount = 1;
    //Se for viável alocar mais de uma thread para processar as respostas, então calcule quantas...
    if (Core::ThreadCount > 1) {
        workerThreadsCount = Core::ThreadCount - listenThreadsCount;
    }

    std::vector<std::shared_ptr<std::thread>> workerThreads;

    Core::Running = true;
    Core::Server = std::shared_ptr<HttpServer>(this);

    //Um thread pool para aceitar conexões (std::this_thread) e as demais threads processam respostas.
    try {
        for (unsigned int i = 0; i < workerThreadsCount; ++i) {
            workerThreads.push_back(std::make_shared<std::thread>(&HttpServer::run, this));
            workerThreads.back()->detach();
        }
    }
    catch (const std::exception &e) {
        std::string description = "Failed spawning worker threads on HttpServer::run.\nDetails: \"";
        description.append(e.what());
        description.append("\"\n");

        Core::warning(description, "void HttpServer::start()");
    }

    Core::outLn("Waiting for HTTP requests on port " + std::to_string(port_) + "...\n");

    IncomingConnection conn;
    unsigned int free_slots = 0;

    while (Core::Running) {
        conn.incoming_socket = INVALID_SOCKET;

        while (conn.incoming_socket == INVALID_SOCKET) {
            free_slots = free_connection_slots_;

            if (free_slots > 0) {
                errno = 0;

                conn.incoming_socket = accept(
                        server_socket_,
                        (struct sockaddr *) &conn.client_address,
                        &conn.client_length
                );

                if (errno == EWOULDBLOCK || errno == EAGAIN || errno == ECONNABORTED) {
                    conn.incoming_socket = INVALID_SOCKET;
                }
            }

            if (this->connections_queue_.empty()) {
                this->putCurrentThreadToSleep();
            }
        }

        --free_connection_slots_;
        ++request_count_;

        if (request_count_ % this->announce_rate_ == 0) {
            Core::debugLn("Request count: " + std::to_string(request_count_));
        }

        this->connections_queue_.push(conn);
    }

    return (int) Status::Ok;
}

void HttpServer::run() {
    IncomingConnection conn;

    while (Core::Running) {
        //Set an INVALID_SOCKET value to the object which represents incoming connections
        conn.incoming_socket = INVALID_SOCKET;

        Core::ThreadMutex.lock();

        //If any client is waiting for processing
        if (!this->connections_queue_.empty()) {
            //Uses a FIFO approach, getting first element from the queue.
            conn = this->connections_queue_.front();

            //Free this connection slot
            this->connections_queue_.pop();
            ++free_connection_slots_;
        }

        Core::ThreadMutex.unlock();

        if (conn.incoming_socket == INVALID_SOCKET) {
            if (this->connections_queue_.empty()) {
                this->putCurrentThreadToSleep();
            }

            continue;
        }

        try {
            this->handle(conn);
        }
        catch (const std::exception &e) {
            throw std::runtime_error("Falha ao processar resposta ao cliente http.");
        }
    }
}

void HttpServer::putCurrentThreadToSleep() const {
    const auto ms_sleep = std::chrono::milliseconds(5);
    std::this_thread::sleep_for(ms_sleep);
}

void HttpServer::handle(const IncomingConnection &conn) {
    try {
        const auto httpRequest = this->receiveRequest(conn);

        this->sendResponse(conn, httpRequest);
    } catch (const std::exception &exception) {
        Core::debugLn(String("Socket closed: ") + exception.what());
        CLOSE_SOCKET(conn.incoming_socket);
    }
}

HttpRequest HttpServer::receiveRequest(const IncomingConnection &conn) {
    char recvBuffer[Core::kBufferSize];

    //Retrieve the client ip address and announce it when debugging
    std::string clientIpAddress = inet_ntoa(conn.client_address.sin_addr);
    Core::debugLn("\nHandling request from " + clientIpAddress + "...");

    bool retry = false;
    //errno = 0;
    String receivedData = "";

    do {
        ssize_t bytesReceived = recv(conn.incoming_socket, recvBuffer, Core::kBufferSize, 0);

        // Client disconnect or error receiving request
        if (bytesReceived <= 0 /*|| errno != 0*/) {
            throw FailedReceivingData();
        }

        // The request should not be fully received yet, let's read some more data
        if (bytesReceived == Core::kBufferSize) {
            retry = true;
        }

        // Store the current buffered data to join request parts
        receivedData.append(recvBuffer);
    } while (retry);

    return HttpRequest(receivedData, clientIpAddress);
}

void HttpServer::sendResponse(const IncomingConnection &conn, const HttpRequest &request) {
    String response = HttpServer::process(request);

    if (response.length() > 0) {
        ssize_t bytesSent;

        do {
            bytesSent = send(conn.incoming_socket, response.data(), response.length(), MSG_NOSIGNAL);
        } while (errno == EWOULDBLOCK);

        CLOSE_SOCKET(conn.incoming_socket);

        Core::debugLn("Bytes Sent: " + std::to_string(bytesSent));
    }

    Core::ThreadMutex.lock();

    ++response_count_;

    if (response_count_ % this->announce_rate_ == 0) {
        Core::debugLn("Response count: " + std::to_string(response_count_));
    }

    Core::ThreadMutex.unlock();
}

String HttpServer::process(const HttpRequest &httpRequest) {
    try {
        //Custom library initializer
        std::shared_ptr<CustomLibrary> app(new FileLibrary(httpRequest));

        HttpResponse response = app->getResponse();

        if (response.status == 404) {
            app.reset(new RouterLibrary(httpRequest));

            response = std::move(app->getResponse());
        }

        return response.toString();
    }
    catch (const std::runtime_error &ex) {
        Core::warning(String("Runtime error silenced by HttpServer: ") + ex.what(), "HttpServer::process");
    }
    catch (const std::exception &ex) {
        Core::warning(String("Exception silenced by HttpServer: ") + ex.what(), "HttpServer::process");
    }

    return HttpResponse(500).toString();
}

void HttpServer::destroySocket() {
#ifdef WINDOWS
    WSACleanup();
#endif
}