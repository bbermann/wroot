#pragma once

#define BUFFER_SIZE 1024

#include "CustomLibrary.hpp"
#include <optional>
#include <nlohmann/json.hpp>


class FileLibrary : public CustomLibrary {
public:
    FileLibrary() : CustomLibrary() {}

    ~FileLibrary() override = default;

    Response handle(const Request &request);

    static StringList CacheableTypes;

private:
    static std::optional<Response> searchCacheFor(const std::string &key);

    static void storeCacheFor(const std::string &key, const std::string &type, const Response &response);

    Response getResponse(const std::string &fullPath, std::ifstream &file) const;

    std::string getFileExtension(const std::string &fullPath) const;
};
