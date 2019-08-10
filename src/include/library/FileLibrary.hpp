#ifndef FILELIBRARY_H
#define FILELIBRARY_H

#define BUFFER_SIZE 1024

#include "CustomLibrary.hpp"

class FileLibrary : public CustomLibrary {
public:
    FileLibrary();

    virtual ~FileLibrary();

    virtual String toString() override;

    void setResponseType();

    HttpResponse getResponse() override;

protected:
    String getFileName();

    String getFullPath();

    String getFileExtension();
};

#endif // FILELIBRARY_H
