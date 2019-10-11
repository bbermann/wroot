#include <include/network/http/Response.hpp>
#include <include/library/RouterLibrary.hpp>
#include <stdexcept>

RouterLibrary::RouterLibrary(const Request &request) : CustomLibrary(request) {
    this->response.compressOutput = false;
    this->response.type = "text/html";
}

RouterLibrary::~RouterLibrary() = default;

String RouterLibrary::toString() {
    String routerPluginPath = Core::ApplicationRoot + "/plugins/Router.lua";
    LuaScript routerPlugin(routerPluginPath);

    this->registerRequest(routerPlugin);
    this->registerResponse(routerPlugin);

    routerPlugin.executeScript();
    routerPlugin.execute("handle()");

    return routerPlugin.getOutput();
}

void RouterLibrary::registerRequest(LuaScript &routerPlugin) {
    routerPlugin.context.registerFunction<std::string (Request::*)(const std::string&)>(
            "get",
                    [](Request &request, const std::string &key) {
                        return request.get(key);
                    }
    );
    routerPlugin.context.writeVariable("request", this->request);
}

void RouterLibrary::registerResponse(LuaScript &routerPlugin) {
    routerPlugin.context.registerMember("status", &Response::status);
    routerPlugin.context.registerMember("type", &Response::type);
    routerPlugin.context.registerMember("content", &Response::content);
    routerPlugin.context.registerMember("compressOutput", &Response::compressOutput);
    routerPlugin.context.registerFunction<void (Response::*)(const std::string&, const std::string&)>(
            "set",
                    [](Response &response, const std::string &key, const std::string &value) {
                        response.set(key, value);
                    }
    );
    routerPlugin.context.writeVariable("response", &this->response);
}