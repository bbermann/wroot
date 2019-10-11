#include <include/library/FileLibrary.hpp>
#include <include/exceptions/http/response/NotFound.hpp>
#include <fstream>
#include <stdexcept>

using std::ifstream;
using std::ios;

FileLibrary::FileLibrary(const Request &request) : CustomLibrary(request) {
    // This library implicitly compress the output
    this->response.compressOutput = false;
}

FileLibrary::~FileLibrary() = default;

String FileLibrary::toString() {
    String fileContent, currentLine;
    String fullPath = this->getFullPath();

    this->setResponseType();

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
    String fileName = this->request.getUrl();

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
    String fileExtension = fileName.explode(".").back();
    return fileExtension;
}

void FileLibrary::setResponseType() {
    String fileExtension = this->getFileExtension();

    if (fileExtension == "html") {
        this->response.type = "text/html";
    } else if (fileExtension == "xml") {
        this->response.type = "application/xhtml";
    } else if (fileExtension == "css") {
        this->response.type = "text/css";
    } else if (fileExtension == "js") {
        this->response.type = "text/javascript";
    } else if (fileExtension == "png" || fileExtension == "ico") {
        this->response.type = "image/png";
    } else if (fileExtension == "jpg" || fileExtension == "jpeg") {
        this->response.type = "image/jpeg";
    } else if (fileExtension == "gif") {
        this->response.type = "image/gif";
    } else {
        this->response.type = "application/octet-stream";
    }
}