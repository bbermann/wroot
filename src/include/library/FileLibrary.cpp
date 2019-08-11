#include <include/library/FileLibrary.hpp>
#include <include/exceptions/http/response/NotFound.hpp>
#include <fstream>
#include <stdexcept>

using std::ifstream;
using std::ios;

FileLibrary::FileLibrary(const HttpRequest &request) : CustomLibrary(request) {
    // This library implicitly compress the output
    this->compressedOutput = false;
}

FileLibrary::~FileLibrary() {

}

HttpResponse FileLibrary::getResponse() {
    this->setResponseType();

    return CustomLibrary::getResponse();
}

String FileLibrary::toString() {
    String fileContent, currentLine;
    String fullPath = this->getFullPath();

    Core::debugLn("[FileLibrary] Reading file: " + fullPath);

    ifstream file;

    file.open(fullPath, ios::in | ios::binary);

    if (file.fail()) {
        throw NotFound();
    }

    if (file.is_open()) {
        while (getline(file, currentLine)) {
            fileContent.append(currentLine + LINE_BREAK);
        }

        file.close();
    }

    return fileContent;
}

String FileLibrary::getFileName() {
    String fileName = this->getRequestUrl();

    if (fileName.empty() || fileName == "/") {
        fileName = "/index.html";
    }

    return fileName;
}

String FileLibrary::getFullPath() {
    String fileName = this->getFileName();
    String fullPath = Core::DocumentRoot + fileName;
    return fullPath;
}

String FileLibrary::getFileExtension() {
    String fileName = this->getFileName();
    String file_extension = fileName.explode(".").back();
    return file_extension;
}

void FileLibrary::setResponseType() {
    String fileExtension = this->getFileExtension();

    if (fileExtension == "html") {
        this->responseType = "text/html";
    } else if (fileExtension == "xml") {
        this->responseType = "application/xhtml";
    } else if (fileExtension == "css") {
        this->responseType = "text/css";
    } else if (fileExtension == "js") {
        this->responseType = "text/javascript";
    } else if (fileExtension == "png" || fileExtension == "ico") {
        this->responseType = "image/png";
    } else if (fileExtension == "jpg" || fileExtension == "jpeg") {
        this->responseType = "image/jpeg";
    } else if (fileExtension == "gif") {
        this->responseType = "image/gif";
    } else {
        this->responseType = "application/octet-stream";
    }
}