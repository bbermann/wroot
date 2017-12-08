#include "PhpLibrary.hpp"
#include "../helper/ConsoleLineHelper.hpp"
#include "../helper/FileHelper.hpp"
#include "../helper/EnvironmentBuilder.hpp"

PhpLibrary::PhpLibrary() : CustomLibrary()
{
    this->useCompressedOutput = false;
    this->responseType = "text/html";
}

PhpLibrary::~PhpLibrary()
{

}

String PhpLibrary::toString()
{
    String path = getFullPath();
    if (!path.endsWith(".php")) {
        String tempPath = "public/index.php";

        FileHelper file_helper;
        if (!file_helper.Exists(path + tempPath))
        {
            tempPath = "index.php";
        }

        path.append(tempPath);
    }

    String environment = getEnvironment();
    ConsoleLineHelper cli(environment + " php " + path);
    String response = cli.executeStdOut();
    return response;
}

String PhpLibrary::getFullPath()
{
    return Core::DocumentRoot + getHttpRequest().getUrl();
}

String PhpLibrary::getEnvironment()
{
    EnvironmentBuilder env;
    env.setPair("DOCUMENT_ROOT", Core::DocumentRoot);
    env.setPair("HTTP_REFERER", ""); //Add to request
    env.setPair("REMOTE_ADDR", "127.0.0.1"); //Add to request
    env.setPair("REMOTE_PORT", std::to_string(Core::ServerPort));
    env.setPair("REQUEST_METHOD", request_.getHttpMethod());
    env.setPair("REQUEST_URI", request_.getUrl());
    env.setPair("SERVER_ADDR", Core::ServerAddress);
    env.setPair("SERVER_PORT", std::to_string(Core::ServerPort));
    env.setPair("SERVER_NAME", Core::ServerName);
    env.setPair("SERVER_SOFTWARE", Core::ServerName);
    env.setPair("SERVER_PROTOCOL", Core::ServerProtocol);
    env.setPair("QUERY_STRING", request_.getQueryString());
    return env.toString();
}