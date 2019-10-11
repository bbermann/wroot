#pragma once

#define BUFFER_SIZE 1024

#include "CustomLibrary.hpp"

class FileLibrary : public CustomLibrary {
public:
    explicit FileLibrary(const Request &request);

    virtual ~FileLibrary();

    String toString() override;

    void setResponseType();

protected:
    String getFileName();

    String getFullPath();

    String getFileExtension();
};
