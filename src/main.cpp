/*
wRoot Http Server

Desenvolvido por: Bruno dos Santos Bermann
 */

#include "include/core/HttpServer.hpp"

using namespace std;

String Core::ApplicationPath;
String Core::ExecutablePath;
String Core::PathSeparator;
String Core::ServerAddress;
String Core::ServerName;
String Core::ServerProtocol;
String Core::DocumentRoot;
StringList Core::Parameters;
bool Core::IsDebugging;
bool Core::SafeThreads;
bool Core::UseCompressedOutput;
bool Core::UseBrowserCache;
bool Core::CallBrowserOnStart;
bool Core::Running;
int Core::ThreadCount;
int Core::ServerPort;
mutex Core::ThreadMutex;
shared_ptr<HttpServer> Core::Server;

int main(int argc, const char* argv[])
{
	Core::setEnvironment(argc, argv);

    HttpServer server(Core::ServerPort);
    //Inicia o loop do servidor
    server.start();

	return 0;
}
