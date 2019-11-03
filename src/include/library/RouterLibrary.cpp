#include "RouterLibrary.hpp"
#include <include/helper/LuaScript.hpp>
#include <include/exceptions/lua/LuaScriptException.hpp>

Response RouterLibrary::handle(const Request &request) {
    Response response;

    try {
        LuaScript router("plugins/Router.lua");

        // Registering request struct
        router.context.registerMember("method", &Request::method);
        router.context.registerMember("uri", &Request::uri);
        router.context.registerMember("httpVersionMajor", &Request::httpVersionMajor);
        router.context.registerMember("httpVersionMinor", &Request::httpVersionMinor);
        router.context.registerMember("headers", &Request::headers);
        router.context.registerMember("body", &Request::body);
        router.context.registerFunction("getUrl", &Request::getUrl);
        router.context.registerFunction("getQueryString", &Request::getQueryString);
        router.context.registerFunction("getHeader", &Request::getHeader);
        router.context.registerFunction("getBody", &Request::getBody);
        router.context.writeVariable("request", request);

        // Registering response struct
        router.context.registerMember("httpVersionMajor", &Response::httpVersionMajor);
        router.context.registerMember("httpVersionMinor", &Response::httpVersionMinor);
        router.context.registerMember("status", &Response::status);
        router.context.registerMember("content", &Response::content);
        router.context.registerFunction("getHeader", &Response::getHeader);
        router.context.registerFunction("setHeader", &Response::setHeader);
        router.context.writeVariable("response", &response);

        router.executeScript();
        router.execute("handle()");

        //response = router.context.readVariable<Response>("response");
    } catch (const LuaScriptException &exception) {
        Core::warning(
                exception.what(),
                "RouterLibrary::handle()"
        );
    }

    return response;
}