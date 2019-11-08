#include "FileLibrary.hpp"
#include <fstream>
#include <stdexcept>

using std::ifstream;
using std::ios;

StringList FileLibrary::CacheableTypes = {"html", "htm", "js", "css"};

Response FileLibrary::handle(const Request &request) {
    // Decode url to path.
    std::string requestPath;

    if (!urlDecode(request.uri, requestPath)) {
        return Response::stockResponse(Response::BadRequest);
    }

    // Request path must be absolute and not contain "..".
    if (requestPath.empty() || requestPath[0] != '/' || requestPath.find("..") != std::string::npos) {
        return Response::stockResponse(Response::BadRequest);
    }

    // Remove the arguments from request uri if any, since we don't need them for loading files
    if (requestPath.find_first_of('?') != std::string::npos) {
        requestPath = requestPath.substr(0, requestPath.find_first_of('?'));
    }

    // If path ends in slash (i.e. is a directory) then add "index.html".
    if (requestPath[requestPath.size() - 1] == '/') {
        requestPath += "index.html";
    }

    // Open the file to send back.
    std::string fullPath = Core::DocumentRoot + requestPath;

    if (Core::HasFileCache) {
        std::optional<Response> response = FileLibrary::searchCacheFor(fullPath);

        if (response) {
            return *response;
        }
    }

    Core::debugLn("[FileLibrary] Reading file: " + fullPath);

    std::ifstream file;
    file.open(fullPath.c_str(), std::ios::in | std::ios::binary);

    if (!file) {
        return Response::stockResponse(Response::NotFound);
    }

    // Fill out the reply to be sent to the client.
    return std::move(getResponse(fullPath, file));
}

Response FileLibrary::getResponse(const std::string &fullPath, std::ifstream &file) const {
    Response response;
    response.status = Response::Ok;

    char buf[Core::BufferSize];
    while (file.read(buf, sizeof(buf)).gcount() > 0) {
        response.content.append(buf, file.gcount());
    }

    auto extension = this->getFileExtension(fullPath);

    response.headers.insert({"Content-Type", MimeTypes::extensionToType(extension)});

    if (Core::HasFileCache) {
        storeCacheFor(fullPath, extension, response);
    }

    return response;
}

std::string FileLibrary::getFileExtension(const std::string &fullPath) const {
    std::size_t lastSlashPos = fullPath.find_last_of('/');
    std::size_t lastDotPos = fullPath.find_last_of('.');
    std::string extension;
    if (lastDotPos != std::string::npos && lastDotPos > lastSlashPos) {
        extension = fullPath.substr(lastDotPos + 1);
    }
    return extension;
}

std::optional<Response> FileLibrary::searchCacheFor(const std::string &key) {
    std::optional<Response> response;

    try {
        auto isCached = Core::Cache.find(key);

        if (isCached == Core::Cache.end()) {
            return response;
        }

        auto cached = Core::Cache.at(key);
        response = Response::unserialize(cached);
    } catch (const nlohmann::json::exception &exception) {
        Core::outLn(String("Failed parsing response cache for key '") + key + "': " + exception.what());
    }

    return response;
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