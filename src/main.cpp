/*
wRoot Http Server

Desenvolvido por: Bruno dos Santos Bermann
 */

#include "include/core/HttpServer.hpp"

int main(int argc, const char* argv[])
{
	Core::setEnvironment(argc, argv);

    HttpServer server(Core::ServerPort);
    //Inicia o loop do servidor
    server.start();

	return 0;
}