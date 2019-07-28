#include "PhpLibrary.hpp"
#include "../helper/ConsoleLineHelper.hpp"
#include "../helper/FileHelper.hpp"
#include "../helper/EnvironmentBuilder.hpp"

PhpLibrary::PhpLibrary() : CustomLibrary() {
    this->compressedOutput = false;
    this->responseType = "text/html";
}

PhpLibrary::~PhpLibrary() {
}

String PhpLibrary::toString() {
    String scriptCall = this->getScriptPathWithUrl();
    String scriptPath = this->getScriptPath();
    String environment = this->getEnvironment(scriptPath);
    String bootstrapCode = "scripts/php/bootstrap.php";

    String call = environment + " php " + bootstrapCode + " " + scriptCall;
    Core::debugLn(call);

    ConsoleLineHelper cli(call);
    return cli.executeStdOut();
}

String PhpLibrary::getScriptPath() {
    return Core::DocumentRoot + this->urlRewriter.getScriptPath(this->getHttpRequest().getUrl());
}

String PhpLibrary::getScriptPathWithUrl() {
    return Core::DocumentRoot + this->urlRewriter.rewrite(this->getHttpRequest().getUrl());
}

String PhpLibrary::getEnvironment(String scriptPath) {
    auto request = this->getHttpRequest();

    EnvironmentBuilder env;
    env.setPair("DOCUMENT_ROOT", Core::DocumentRoot);
    env.setPair("SCRIPT_FILENAME", scriptPath);
    env.setPair("HTTP_METHOD", request.getHttpMethod());
    env.setPair("HTTP_REFERER", request.get("HTTP_REFERER"));
    env.setPair("REMOTE_ADDR", request.get("REMOTE_ADDR"));
    env.setPair("REMOTE_PORT", std::to_string(Core::ServerPort));
    env.setPair("REQUEST_METHOD", request.getHttpMethod());
    env.setPair("REQUEST_URI", request.getUrl());
    env.setPair("SERVER_ADDR", Core::ServerAddress);
    env.setPair("SERVER_PORT", std::to_string(Core::ServerPort));
    env.setPair("SERVER_NAME", Core::ServerName);
    env.setPair("SERVER_SOFTWARE", Core::ServerName);
    env.setPair("SERVER_PROTOCOL", Core::ServerProtocol);
    env.setPair("QUERY_STRING", request.get("QUERY_STRING"));
    return env.toString();
}

String PhpLibrary::getSessionScript() {
    auto request = this->getHttpRequest();
    auto queryString = request.get("QUERY_STRING");
    auto method = request.get("HTTP_METHOD");
    auto vars = queryString.explode("&");

    String output = "--process-begin \"{code}\"";
    String script = "";

    for (String var : vars) {
        auto parts = var.explode("=");

        if (parts.size() > 0) {
            String val = "true";

            if (parts.size() > 1) {
                val = parts.at(1);
            }

            script.append("$_" + method + "['" + parts.at(0) + "'] = '" + val + "';" + ENDL);
        }
    }

    if (script.empty())
        return "";

    output = String::replace(output, "{code}", script);
    Core::debugLn("[PHP BOOTSTRAP SCRIPT]: " + script);
    return output;
}