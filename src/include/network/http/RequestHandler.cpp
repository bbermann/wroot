//
// Created by bbermann on 10/10/2019.
//

#include "RequestHandler.hpp"
#include "../../core/Core.hpp"
#include <include/core/Core.hpp>
#include <include/library/FileLibrary.hpp>
#include <include/library/RouterLibrary.hpp>

RequestHandler::RequestHandler() = default;

void RequestHandler::handle(const Request &request, Response &response) {
    static FileLibrary fileLibrary;
    static RouterLibrary routerLibrary;

    response = std::move(fileLibrary.handle(request));

    if (response.status == Response::NotFound) {
        response = std::move(routerLibrary.handle(request));
    }

    this->finishResponse(request, response);
}

void RequestHandler::finishResponse(const Request &request, Response &response) {
    if (response.headers.find("X-Powered-By") == response.headers.end()) {
        response.headers["X-Powered-By"] = "wRoot";
    }

    if (response.headers.find("Host") == response.headers.end()) {
        response.headers["Host"] = Core::ServerAddress;
    }

    if (response.headers.find("Server") == response.headers.end()) {
        response.headers["Server"] = Core::ServerName;
    }

    if (response.headers.find("Content-Type") == response.headers.end()) {
        response.headers["Content-Type"] = "application/octet-stream";
    }

    if (response.headers.find("Content-Length") == response.headers.end()) {
        response.headers["Content-Length"] = std::to_string(response.content.length());
    }

    if (response.headers.find("Date") == response.headers.end()) {
        response.headers["Date"] = "Thu, 08 Oct 2015 16:42:10 GMT";
    }

//    response.headers["Connection"] = request.headers["Connection"] ?? "close";

//    if (this->compressOutput) {
//        //returnString.append("Transfer-Encoding: gzip" + ENDL);
//
//        this->content = ZLib::compress_string(this->content);
//        returnString.append("Content-Encoding: deflate" + ENDL);
//    }

//    returnString.append("Vary: Accept-Encoding, Cookie, User-Agent" + ENDL);
//    returnString.append("Pragma: public" + ENDL);
//    returnString.append("Expires: Sat, 28 Nov 2009 05:36:25 GMT" + ENDL);
//    returnString.append("Last-Modified: Sat, 28 Nov 2009 03:50:37 GMT" + ENDL);
//    returnString.append("X-Pingback: http://net.tutsplus.com/xmlrpc.php" + ENDL);
}