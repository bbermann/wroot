#include "FileLibrary.hpp"
#include <include/exceptions/http/response/NotFound.hpp>
#include <include/network/http/MimeTypes.hpp>
#include <fstream>
#include <stdexcept>

using std::ifstream;
using std::ios;

StringList FileLibrary::CacheableTypes = {"html", "htm", "js", "css"};

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

    // Remove the arguments from request uri if any, since we don't need them for loading files
    if (requestPath.find_first_of('?') != std::string::npos) {
        requestPath = requestPath.substr(0, requestPath.find_first_of('?'));
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

    if (Core::HasFileCache && this->searchCacheFor(fullPath, response)) {
        return;
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
    response.headers[0].key = "Content-Length";
    response.headers[0].value = std::to_string(response.content.size());
    response.headers[1].key = "Content-Type";
    response.headers[1].value = MimeTypes::extensionToType(extension);

    if (Core::HasFileCache) {
        FileLibrary::storeCacheFor(fullPath, extension, response);
    }
}

bool FileLibrary::searchCacheFor(const std::string &key, Response &response) {
    try {
        auto isCached = Core::Cache.find(key);

        if (isCached == Core::Cache.end()) return false;

        auto cached = Core::Cache.at(key);
        response = Response::unserialize(cached);

        return true;
    } catch (const nlohmann::json::exception &exception) {
        Core::outLn(String("Failed parsing response cache for key '") + key + "': " + exception.what());
    }

    return false;
}

void FileLibrary::storeCacheFor(const std::string &key, const std::string &type, const Response &response) {
    try {
        auto found = std::find(FileLibrary::CacheableTypes.begin(), FileLibrary::CacheableTypes.end(), type);

        if (found != FileLibrary::CacheableTypes.end()) {
            Core::Cache[key] = response.serialize();
        }
    } catch (const std::exception &exception) {
        Core::outLn(
                String("Failed storing response cache for key '") + key + "' and type '" + type + "': " +
                exception.what()
        );
    }
}