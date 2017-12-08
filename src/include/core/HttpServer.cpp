#include "HttpServer.hpp"
#include "HttpRequest.hpp"
#include "../type/Html.hpp"
#include "../type/Timer.hpp"
#include "../type/Exception.hpp"
#include "../type/Process.hpp"
#include "../helper/FileHelper.hpp"
#include "../library/FileLibrary.hpp"
#include "../library/PhpLibrary.hpp"

#include <mutex>
#include <exception>

#ifdef WINDOWS
    #undef errno
    #define errno WSAGetLastError()
    #ifndef EWOULDBLOCK
        #define EWOULDBLOCK WSAEWOULDBLOCK
    #endif
#else
    #include <fcntl.h>    /* For O_RDWR */
    #include <unistd.h>   /* For open(), creat() */
#endif

using namespace std;

vector<HtmlElement> HtmlElement::templates;

HttpServer::HttpServer(size_t port)
{
    port_ = port;
    server_socket_ = INVALID_SOCKET;

    if (!this->createSocket()) exit(2);

    free_connection_slots_ = Core::kMaxConnections;

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

		const char* reuseSocket = "1";
		setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, reuseSocket, sizeof(reuseSocket));
		//setsockopt(server_socket_, SOL_SOCKET, SO_KEEPALIVE, reuseSocket, sizeof(reuseSocket));

		// If iMode!=0, non-blocking mode is enabled.
		u_long iMode = 1;
		ioctlsocket(server_socket_, FIONBIO, &iMode);

        if (::bind(server_socket_, (SOCKADDR*) & server_address_, sizeof (server_address_)) == SOCKET_ERROR)
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
        setsockopt(server_socket_, SOL_SOCKET, SO_REUSEPORT, &reuseSocket, sizeof (reuseSocket));
        setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &reuseSocket, sizeof (reuseSocket));
        setsockopt(server_socket_, SOL_SOCKET, SO_KEEPALIVE, &reuseSocket, sizeof (reuseSocket));

        int fcntl_status = fcntl(server_socket_, F_SETFL, fcntl(server_socket_, F_GETFL, 0) /*| O_NONBLOCK*/);

        if (fcntl_status == -1)
        {
            Core::error("Failed to set non blocking option on the server socket.", "bool HttpServer::createSocket()");
        }

        if (bind(server_socket_, (struct sockaddr*) &server_address_, sizeof (server_address_)) == SOCKET_ERROR)
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
        Core::error("Error listening on socket at port " + to_string(port_) + ".");
        return;
    }

    request_count_ = 0;
    response_count_ = 0;

    //Uma thread (this_thread) é reservada para escutar a porta de entrada.
    const unsigned int listen_threads = 1;
    //Ao menos uma thread é necessária para processar as respostas...
    int worker_threads = 1;
    //Se for viável alocar mais de uma thread para processar as respostas, então calcule quantas...
    if (Core::ThreadCount > 1)
    {
        worker_threads = Core::ThreadCount - listen_threads;
    }

    thread** serverThreads = new thread*[worker_threads]();

    Core::Running = true;
    Core::Server = shared_ptr<HttpServer>(this);

    //Um thread pool para aceitar conexões (this_thread) e as demais threads processam respostas.
    try
    {
        for (int threadIterator = 0; threadIterator < worker_threads; threadIterator++)
        {
            serverThreads[threadIterator] = new thread(&HttpServer::run, this);
            serverThreads[threadIterator]->detach();
        }
    }
    catch (exception e)
    {
        string error_descriptor = "Falha na execução assíncrona de HttpServer::run.\nDetalhes: \"";
        error_descriptor.append(e.what());
        error_descriptor.append("\"\n");

        Core::warning(error_descriptor, "void HttpServer::start()");
    }

    Core::outLn("wRoot started successfully.");
    Core::outLn("Waiting for incoming connections at port " + to_string(port_) + "...");

    if (Core::CallBrowserOnStart)
    {
        Core::outLn("Spawning default browser...");
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
                conn.incomming_socket = accept(server_socket_, (struct sockaddr *) &conn.client_address, &conn.client_length);

                if (errno == EWOULDBLOCK)
                {
                    conn.incomming_socket = INVALID_SOCKET;
                }
            }

            ms_sleep = this->client_pending_.size() > 0 ? 0 : 10;
            this_thread::sleep_for(chrono::milliseconds(ms_sleep));
        }

        --free_connection_slots_;
        ++request_count_;

        //O número de requisições é diferente do número de respostas quando conexões são canceladas.
        if (request_count_ % 1000 == 0)
        {
            Core::outLn("Request count: " + to_string(request_count_));
        }
        else
        {
            Core::debugLn("Request count: " + to_string(request_count_));
        }

        if (Core::IsDebugging)
        {
            Core::debugLn("\n--------------------------------------------------------------------------------");
            Core::debugLn("Request count: " + to_string(request_count_));
        }

		this->client_pending_.push(conn);
    }

    //Desaloca a memória reservada para as threads do servidor.
    for (int it = 0; it < worker_threads; ++it)
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
		//Invalidado o objeto que representa a conexão de entrada
        conn.incomming_socket = INVALID_SOCKET;

        if (Core::SafeThreads) Core::ThreadMutex.lock();

		//Se houver algum cliente na fila de espera
        if (this->client_pending_.size() > 0)
        {
			//Atende a primeira conexão de entrada na fila
			conn = this->client_pending_.front();
            
			//Libera o slot de conexão reservado ao cliente atendido
			this->client_pending_.pop();
			++free_connection_slots_;
        }

        if (Core::SafeThreads) Core::ThreadMutex.unlock();

        if (conn.incomming_socket == INVALID_SOCKET)
        {
            ms_sleep = this->client_pending_.size() > 0 ? 0 : 1;
            this_thread::sleep_for(chrono::milliseconds(ms_sleep));
            continue;
        }

        try
        {
			Timer timer;

            if (Core::IsDebugging)
            {
				timer.start();
            }

            //Processa requisição e envia a resposta ao cliente
            response(conn);

            if (Core::IsDebugging)
            {
                Core::debugLn(to_string(timer.finish()) + " microsegundos se passaram desde o inicio de wRoot.response().\n");
            }
        }
        catch (exception e)
        {
            throw Exception("Falha ao processar resposta ao cliente http.", "void HttpServer::run()");
        }
    }
}

void HttpServer::response(IncommingConnection& conn)
{
    char recvBytes[Core::kBufferSize];
    string conn_url, conn_method;
    string client_ipaddress = inet_ntoa(conn.client_address.sin_addr);
    ssize_t bytesRecv = -1;

    if (Core::IsDebugging)
    {
        if (Core::SafeThreads) Core::ThreadMutex.lock();

        Core::debugLn("\n--------------------------------------------------------------------------------");
        Core::debugLn("Responding request from " + client_ipaddress + "...");
        Core::debugLn("Size of request queue is " + to_string(client_pending_.size()) + ".");

        if (Core::SafeThreads) Core::ThreadMutex.unlock();
    }

	bytesRecv = recv(conn.incomming_socket, recvBytes, Core::kBufferSize, 0);

    if (bytesRecv <= 0)
    {
        Core::debugLn("Socket closed.");
        CLOSESOCKET(conn.incomming_socket);
        return;
    }

    //URL Explain
	String request = recvBytes;

	//Processa a resposta a ser enviada ao cliente
	String response = process(request);

	if (response.length() > 0)
    {
        ssize_t bytesSent = -1;

		do
		{
			bytesSent = send(conn.incomming_socket, response.data(), response.length(), MSG_NOSIGNAL);
		} while (errno == EWOULDBLOCK);

		CLOSESOCKET(conn.incomming_socket);

		/*Core::debugLn("Message Sent:" + tmpStr);
		Core::debugLn("");*/
        Core::debugLn("Bytes Sent: " + to_string(bytesSent));
    }

    if (Core::SafeThreads) Core::ThreadMutex.lock();

    ++response_count_;

    if (response_count_ % 1000 == 0)
    {
        Core::outLn("Response count: " + to_string(response_count_));
    }
    else
    {
        Core::debugLn("Response count: " + to_string(response_count_));
    }

    if (Core::SafeThreads) Core::ThreadMutex.unlock();
}

String HttpServer::process(String request)
{
    Timer timer;
    timer.start();

    HttpRequest httpRequest(request);
    
    FileHelper fileHelper;
    String fileName = Core::ApplicationPath + httpRequest.getUrl();

    //Custom library initializer
	unique_ptr<CustomLibrary> app(new FileLibrary());

    if (!fileHelper.Exists(Core::DocumentRoot + fileName) || fileHelper.CheckExtension(fileName, "php"))
    {
        app.reset(new PhpLibrary());
    }

    app->setHttpRequest(httpRequest);
	
    HttpResponse response = app->getResponse();

    Core::debugLn(to_string(timer.finish()) + " microsegundos se passaram desde o inicio de ExecAPI.");

    return response.toString();
}

void HttpServer::getUrl(String& url, String& library, String& function, StringList& arguments)
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

	if (split.size() > 0) return split.at(0);

	return "";
}

bool HttpServer::getHeaderParameter(String header, HeaderParameter parameter)
{
	switch (parameter)
	{
	case HeaderParameter::ConnectionKeepAlive:
		return header.contains("Connection: keep-alive");

	case HeaderParameter::ConnectionClose:
		return 
			header.contains("Connection: close") ||
			header.contains("Connection: keep-alive, close");
	}

	return false;
}
