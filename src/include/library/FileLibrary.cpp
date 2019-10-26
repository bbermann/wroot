#include "FileLibrary.hpp"
#include <include/exceptions/http/response/NotFound.hpp>
#include <include/network/http/MimeTypes.hpp>
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>

using std::ifstream;
using std::ios;

FileLibrary::FileLibrary(const Request &request) : CustomLibrary(request) {
}

FileLibrary::~FileLibrary() = default;

void FileLibrary::handle(Response &response) {
    // Decode url to path.
    std::string requestPath;

    if (!urlDecode(request.uri, requestPath)) {
        response = Response::stockResponse(Response::BadRequest);
        return;
    }

    // Request path must be absolute and not contain "..".
    if (requestPath.empty() || requestPath[0] != '/' || requestPath.find("..") != std::string::npos) {
        response = Response::stockResponse(Response::BadRequest);
        return;
    }

    // If path ends in slash (i.e. is a directory) then add "index.html".
    if (requestPath[requestPath.size() - 1] == '/') {
        requestPath += "index.html";
    }

    // Determine the file extension.
    std::size_t lastSlashPos = requestPath.find_last_of('/');
    std::size_t lastDotPos = requestPath.find_last_of('.');
    std::string extension;
    if (lastDotPos != std::string::npos && lastDotPos > lastSlashPos) {
        extension = requestPath.substr(lastDotPos + 1);
    }

    // Open the file to send back.
    std::string fullPath = Core::DocumentRoot + requestPath;

    try {
        auto isCached = Core::Cache.find(fullPath);

        if (isCached != Core::Cache.end()) {
            auto cached = Core::Cache.at(fullPath);

            response = Response::unserialize(cached);
            return;
        }
    } catch (const nlohmann::json::exception &exception) {
        Core::outLn(String("Failed parsing response cache for file ") + fullPath + ": " + exception.what());
    }

    Core::debugLn("[FileLibrary] Reading file: " + fullPath);

    std::ifstream file;
    file.open(fullPath.c_str(), std::ios::in | std::ios::binary);

    if (!file) {
        response = Response::stockResponse(Response::NotFound);
        return;
    }

    // Fill out the reply to be sent to the client.
    response.status = Response::Ok;
    char buf[Core::BufferSize];

    while (file.read(buf, sizeof(buf)).gcount() > 0) {
        response.content.append(buf, file.gcount());
    }

    response.headers.resize(2);
    response.headers[0].name = "Content-Length";
    response.headers[0].value = std::to_string(response.content.size());
    response.headers[1].name = "Content-Type";
    response.headers[1].value = MimeTypes::extensionToType(extension);

    // If the file extension should be cached...
    auto found = std::find(this->cacheableExtensions_.begin(), this->cacheableExtensions_.end(), extension);
    if (found != this->cacheableExtensions_.end()) {
        Core::Cache[fullPath] = response.serialize();
    }
}