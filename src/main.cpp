/*
wRoot Http Server

Desenvolvido por: Bruno dos Santos Bermann
 */

#include <include/core/Core.hpp>
#include <include/core/HttpServer.hpp>

int main(int argc, const char *argv[]) {
    try {
        Core::setEnvironment(argc, argv);

        HttpServer server(Core::ServerListenAddress, Core::ServerPort);
        server.run();
    } catch (const std::exception &exception) {
        Core::error(exception.what());
    }
}