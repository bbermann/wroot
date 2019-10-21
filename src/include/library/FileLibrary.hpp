#pragma once

#define BUFFER_SIZE 1024

#include "CustomLibrary.hpp"

class FileLibrary : public CustomLibrary {
public:
    explicit FileLibrary(const Request &request);
    virtual ~FileLibrary();

    void handle(Response &response);

private:
    StringList cacheableExtensions_ = {"html", "htm", "js", "css"};
};
