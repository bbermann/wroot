#define WROOT_USE_CLUSTERLIBRARY

#include "HttpServer.hpp"
#include "HttpRequest.hpp"
#include "../type/Html.hpp"
#include "../type/Timer.hpp"
#include "../type/Exception.hpp"
#include "../type/Process.hpp"
#include "../helper/FileHelper.hpp"
#include "../library/FileLibrary.hpp"

#if defined(WROOT_USE_PHPLIBRARY)
#include "../library/PhpLibrary.hpp"
#endif

#if defined(WROOT_USE_CLUSTERLIBRARY)
#include "../library/ClusterLibrary.hpp"
#endif

#include <mutex>
#include <exception>
#include <future>
#include <thread>

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

using namespace std;

vector<HtmlElement> HtmlElement::templates;

HttpServer::HttpServer(size_t port)
{
    this->port_ = port;
    this->server_socket_ = INVALID_SOCKET;

    if (!this->createSocket())
        exit(2);

    this->free_connection_slots_ = Core::kMaxConnections;
    this->announce_rate_ = Core::IsDebugging ? 10 : 1000;

    HtmlElement::createTemplates();
}

HttpServer::~HttpServer()
{
    this->destroySocket();
}

bool HttpServer::createSocket()
{
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
            CLOSESOCKET(server_socket_);
            bReturn = false;
        }
    }
#else
    server_socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server_socket_ == INVALID_SOCKET)
    {
        Core::error("Create socket failed!");
    }
    else
    {
        server_address_.sin_family = AF_INET;
        server_address_.sin_addr.s_addr = INADDR_ANY;
        server_address_.sin_port = htons(port_);

        int reuseSocket = 1;
        setsockopt(server_socket_, SOL_SOCKET, SO_REUSEPORT, &reuseSocket, sizeof(reuseSocket));
        setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &reuseSocket, sizeof(reuseSocket));
        setsockopt(server_socket_, SOL_SOCKET, SO_KEEPALIVE, &reuseSocket, sizeof(reuseSocket));

        int fcntl_status = fcntl(server_socket_, F_SETFL, fcntl(server_socket_, F_GETFL, 0) /*| O_NONBLOCK*/);

        if (fcntl_status == -1)
        {
            Core::error("Failed to set non blocking option on the server socket.", "bool HttpServer::createSocket()");
        }

        if (bind(server_socket_, (struct sockaddr *)&server_address_, sizeof(server_address_)) == SOCKET_ERROR)
        {
            Core::error("Socket binding has failed.");
            CLOSESOCKET(server_socket_);
            bReturn = false;
        }
        else
        {
            bReturn = true;
        }
    }
#endif

    return bReturn;
}

void HttpServer::start()
{
    if (listen(server_socket_, Core::kMaxConnections) == SOCKET_ERROR)
    {
        Core::error("Unable to open socket on port " + to_string(port_) + ".");
        return;
    }

    request_count_ = 0;
    response_count_ = 0;

    //Uma thread (this_thread) é reservada para escutar a porta de entrada.
    const unsigned int listenThreads = 1;
    //Ao menos uma thread é necessária para processar as respostas...
    unsigned int workerThreads = 1;
    //Se for viável alocar mais de uma thread para processar as respostas, então calcule quantas...
    if (Core::ThreadCount > 1)
    {
        workerThreads = Core::ThreadCount - listenThreads;
    }

    thread **serverThreads = new thread *[workerThreads]();

    Core::Running = true;
    Core::Server = shared_ptr<HttpServer>(this);

    //Um thread pool para aceitar conexões (this_thread) e as demais threads processam respostas.
    try
    {
        for (int threadIterator = 0; threadIterator < workerThreads; threadIterator++)
        {
            serverThreads[threadIterator] = new thread(&HttpServer::run, this);
            serverThreads[threadIterator]->detach();
        }
    }
    catch (exception e)
    {
        string description = "Failed spawning worker threads on HttpServer::run.\nDetails: \"";
        description.append(e.what());
        description.append("\"\n");

        Core::warning(description, "void HttpServer::start()");
    }

    Core::outLn("Waiting for HTTP requests on port " + to_string(port_) + "...\n");

    if (Core::CallBrowserOnStart)
    {
        Core::outLn("Starting Google Chrome...");
#ifdef WINDOWS
        Process browser("chrome", "http://localhost:8080/");
#else
        Process browser("google-chrome", "http://localhost:8080/");
#endif
        browser.runAsync();
    }

    IncommingConnection conn;
    int ms_sleep = 0;
    int free_slots = 0;

    while (Core::Running)
    {
        conn.incomming_socket = INVALID_SOCKET;

        while (conn.incomming_socket == INVALID_SOCKET)
        {
            free_slots = free_connection_slots_;

            if (free_slots > 0)
            {
                conn.incomming_socket = accept(server_socket_, (struct sockaddr *)&conn.client_address, &conn.client_length);

                if (errno == EWOULDBLOCK || errno == EAGAIN || errno == ECONNABORTED)
                {
                    conn.incomming_socket = INVALID_SOCKET;
                }
            }

            ms_sleep = this->client_pending_.size() > 0 ? 0 : 5;
            this_thread::sleep_for(chrono::milliseconds(ms_sleep));
        }

        --free_connection_slots_;
        ++request_count_;

        if (request_count_ % this->announce_rate_ == 0)
        {
            Core::outLn("Request count: " + to_string(request_count_));
        }

        this->client_pending_.push(conn);
    }

    //Shutdown...
    //Free the server threads memory
    for (int it = 0; it < workerThreads; ++it)
    {
        delete serverThreads[it];
    }
}

void HttpServer::run()
{
    IncommingConnection conn;
    int ms_sleep = 0;

    while (Core::Running)
    {
        //Set an INVALID_SOCKET value to the object which represents incomming connections
        conn.incomming_socket = INVALID_SOCKET;

        Core::ThreadMutex.lock();

        //If any client is waiting for processing
        if (this->client_pending_.size() > 0)
        {
            //Uses a FIFO approach, getting first element from the queue.
            conn = this->client_pending_.front();

            //Free this connection slot
            this->client_pending_.pop();
            ++free_connection_slots_;
        }

        Core::ThreadMutex.unlock();

        if (conn.incomming_socket == INVALID_SOCKET)
        {
            ms_sleep = this->client_pending_.size() > 0 ? 0 : 1;
            this_thread::sleep_for(chrono::milliseconds(ms_sleep));
            continue;
        }

        try
        {
            //Process the request and send the response to the client
            this->response(conn);
        }
        catch (exception e)
        {
            throw Exception("Falha ao processar resposta ao cliente http.", "void HttpServer::run()");
        }
    }
}

void HttpServer::response(IncommingConnection &conn)
{
    Timer timer;

    std::future<HttpRequest> httpRequestFuture = std::async([conn, &timer] {
        timer.start();

        char recvBuffer[Core::kBufferSize];

        //Retrieve the client ip address and announce it when debugging
        string clientIpAddress = inet_ntoa(conn.client_address.sin_addr);
        Core::debugLn("\nHandling request from " + clientIpAddress + "...");
        
        bool retry = false;
        errno = 0;
        String receivedData = "";

        do {
            ssize_t bytesReceived = recv(conn.incomming_socket, recvBuffer, Core::kBufferSize, 0);

            // Client disconnect or error receiving request
            if (bytesReceived <= 0 || errno != 0)
            {
                // TODO: Add error log
                return HttpRequest();
            }

            // The request should not be fully received yet, let's read some more data
            if (bytesReceived == Core::kBufferSize) {
                retry = true;
            }

            // Store the current buffered data to join request parts 
            receivedData.append(recvBuffer);
        } while (retry);

        Core::debugLn("Request received in " + to_string(timer.finish()) + "ms.");

        HttpRequest httpRequest(receivedData, clientIpAddress);
        return httpRequest;
    });
    auto httpRequest = httpRequestFuture.get();

    if (!httpRequest.isValid()) {
        Core::debugLn("Socket closed.");
        CLOSESOCKET(conn.incomming_socket);
    }

    //Process request and get response    
    std::future<String> responseFuture = std::async([this, &httpRequest, &timer] { 
        timer.start();
        auto response = this->process(httpRequest);
        Core::debugLn("Request processed in " + to_string(timer.finish()) + "ms.");
        return response;
    });
    String response = responseFuture.get();
    //String response = this->process(httpRequest);

    if (response.length() > 0)
    {
        ssize_t bytesSent = -1;

        timer.start();

        do
        {
            bytesSent = send(conn.incomming_socket, response.data(), response.length(), MSG_NOSIGNAL);
        } while (errno == EWOULDBLOCK);

        CLOSESOCKET(conn.incomming_socket);

        Core::debugLn("Response sent in " + to_string(timer.finish()) + "ms.");
        Core::debugLn("Bytes Sent: " + to_string(bytesSent));
    }

    Core::ThreadMutex.lock();

    ++response_count_;

    if (response_count_ % this->announce_rate_ == 0)
    {
        Core::outLn("Response count: " + to_string(response_count_));
    }

    Core::ThreadMutex.unlock();
}

String HttpServer::process(HttpRequest httpRequest)
{
    try
    {
        String url = httpRequest.getUrl();
        Core::debugLn("Requested URL: " + url);

        FileHelper fileHelper;
        String fileName = Core::ApplicationPath + url;

        //Custom library initializer
        unique_ptr<CustomLibrary> app(new FileLibrary());

#if defined(WROOT_USE_CLUSTERLIBRARY)
        app.reset(new ClusterLibrary());
#elif defined(WROOT_USE_PHPLIBRARY)
        if (!fileHelper.Exists(Core::DocumentRoot + fileName) || fileHelper.CheckExtension(fileName, "php"))
        {
            app.reset(new PhpLibrary());
        }
#endif

        app->setHttpRequest(httpRequest);

        HttpResponse response = app->getResponse();
        return response.toString();
    }
    catch (std::runtime_error ex)
    {
        Core::warning(String("Runtime error silenced by HttpServer: ") + ex.what(), "HttpServer::process");
    }
    catch (std::exception ex)
    {
        Core::warning(String("Exception silenced by HttpServer: ") + ex.what(), "HttpServer::process");
    }

    return Core::httpError((unsigned short)500).toString();
}

void HttpServer::getUrl(String &url, String &library, String &function, StringList &arguments)
{
    //Conversão do url em array (variável expurl), cada elemento é separado por uma barra (/).
    StringList expurl = url.explode("/");

    for (StringList::iterator i = expurl.begin(); i != expurl.end(); i++)
    {
        //Primeiro elemento => LIBRARY
        if (i == expurl.begin())
        {
            library = *i;
            library = String::toLower(library);
            continue;
        }

        //Segundo elemento => FUNCTION
        if (i == expurl.begin() + 1)
        {
            function = *i;
            function = String::toLower(function);
            continue;
        }

        /*	Demais elementos deverão ser absorvidos pelo
        array "arguments" a cada iteração. */
        arguments.push_back(*i);
    }
}

void HttpServer::destroySocket()
{
#ifdef WINDOWS
    WSACleanup();
#endif
}

String HttpServer::getHttpHeader(String request)
{
    StringList split = request.explode("\r\n\r\n");

    if (split.size() > 0)
        return split.at(0);

    return "";
}

bool HttpServer::getHeaderParameter(String header, HeaderParameter parameter)
{
    switch (parameter)
    {
    case HeaderParameter::ConnectionKeepAlive:
        return header.contains("Connection: keep-alive");

    case HeaderParameter::ConnectionClose:
        return header.contains("Connection: close") ||
               header.contains("Connection: keep-alive, close");
    }

    return false;
}
