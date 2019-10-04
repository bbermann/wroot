#include <include/core/HttpResponse.hpp>
#include <include/library/RouterLibrary.hpp>
#include <stdexcept>

RouterLibrary::RouterLibrary(const HttpRequest &request) : CustomLibrary(request) {
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
    routerPlugin.context.registerFunction<std::string (HttpRequest::*)(const std::string&)>(
            "get",
                    [](HttpRequest &request, const std::string &key) {
                        return request.get(key);
                    }
    );
    routerPlugin.context.writeVariable("request", this->request);
}

void RouterLibrary::registerResponse(LuaScript &routerPlugin) {
    routerPlugin.context.registerMember("status", &HttpResponse::status);
    routerPlugin.context.registerMember("type", &HttpResponse::type);
    routerPlugin.context.registerMember("content", &HttpResponse::content);
    routerPlugin.context.registerMember("compressOutput", &HttpResponse::compressOutput);
    routerPlugin.context.registerFunction<void (HttpResponse::*)(const std::string&, const std::string&)>(
            "set",
                    [](HttpResponse &response, const std::string &key, const std::string &value) {
                        response.set(key, value);
                    }
    );
    routerPlugin.context.writeVariable("response", &this->response);
}