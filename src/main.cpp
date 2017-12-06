/*
wRoot Http Server

Desenvolvido por: Bruno dos Santos Bermann
 */

#include "include/core/HttpServer.hpp"

using namespace std;

String Core::ApplicationPath;
String Core::ExecutablePath;
String Core::PathSeparator;
StringList Core::Parameters;
bool Core::IsDebugging;
bool Core::SafeThreads;
bool Core::UseCompressedOutput;
bool Core::UseBrowserCache;
bool Core::CallBrowserOnStart;
bool Core::Running;
int Core::ThreadCount;
size_t Core::HttpServerPortNumber;
mutex Core::ThreadMutex;
shared_ptr<HttpServer> Core::Server;

int main(int argc, const char* argv[])
{
	Core::setEnvironment(argc, argv);

    HttpServer server(Core::HttpServerPortNumber);
    //Inicia o loop do servidor
    server.start();

	return 0;
}
