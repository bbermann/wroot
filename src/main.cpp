/*
wRoot Http Server

Desenvolvido por: Bruno dos Santos Bermann
 */

#include <include/core/HttpServer.hpp>

int main(int argc, const char *argv[]) {
    try {
        Core::setEnvironment(argc, argv);

        HttpServer server(Core::ServerPort);

        return server.eventLoop();
    } catch (const std::exception &exception) {
        Core::error(exception.what());
    }
}