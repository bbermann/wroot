#include "PhpLibrary.hpp"
#include "../helper/ConsoleLineHelper.hpp"
#include "../helper/FileHelper.hpp"

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

    ConsoleLineHelper cli("php " + path);
    String response = cli.executeStdOut();
    return response;
}

String PhpLibrary::getFullPath()
{
    return Core::ApplicationPath + getHttpRequest().getUrl();
}