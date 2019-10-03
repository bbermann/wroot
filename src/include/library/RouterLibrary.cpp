#include <include/library/RouterLibrary.hpp>
#include <include/helper/LuaScript.hpp>
#include <stdexcept>

RouterLibrary::RouterLibrary(const HttpRequest &request) : CustomLibrary(request) {
    this->response.compressOutput = false;
    this->response.type = "text/html";
}

RouterLibrary::~RouterLibrary() = default;

String RouterLibrary::toString() {
    String routerPluginPath = Core::ApplicationRoot + "/plugins/Router.lua";
    LuaScript routerPlugin(routerPluginPath);

    routerPlugin.context.registerFunction<std::string (HttpRequest::*)(const std::string&)>(
            "get",
            [](HttpRequest &request, const std::string &key) {
                return request.get(key);
            }
    );
    routerPlugin.context.writeVariable("request", this->request);

    // TODO: Add response (not working in lua scripts simple adding following line of code)
    // routerPlugin.context.writeVariable("response", this->response);

    routerPlugin.executeScript();
    routerPlugin.execute("handle()");

    return routerPlugin.getOutput();
}