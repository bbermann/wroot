#include <include/library/RouterLibrary.hpp>
#include <include/helper/LuaScript.hpp>
#include <stdexcept>

RouterLibrary::RouterLibrary(const HttpRequest &request) : CustomLibrary(request) {
    this->compressedOutput = false;
    this->responseType = "text/html";
}

RouterLibrary::~RouterLibrary() = default;

String RouterLibrary::toString() {
    String routerPluginPath = Core::ApplicationRoot + "/plugins/Router.lua";

    HttpRequest request = this->getHttpRequest();

    LuaScript routerPlugin(routerPluginPath);
    routerPlugin.context.registerFunction<std::string (HttpRequest::*)(const std::string&)>(
            "get",
            [](HttpRequest &request, const std::string &key) {
                return request.get(key);
            }
    );
    routerPlugin.context.writeVariable("request", &request);
    routerPlugin.executeScript();
    routerPlugin.execute("handle()");
    return routerPlugin.getOutput();
}