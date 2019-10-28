#pragma once

#define BUFFER_SIZE 1024

#include "CustomLibrary.hpp"
#include <optional>
#include <nlohmann/json.hpp>


class FileLibrary : public CustomLibrary {
public:
    explicit FileLibrary(const Request &request);

    virtual ~FileLibrary();

    void handle(Response &response);

    static StringList CacheableTypes;

private:
    bool searchCacheFor(const std::string &key, Response &response);
    void storeCacheFor(const std::string &key, const std::string &type, const Response &response);
};
