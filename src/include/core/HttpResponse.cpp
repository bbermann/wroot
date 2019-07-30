/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HttpContent.cpp
 * Author: bbermann
 * 
 * Created on 7 de Fevereiro de 2016, 00:57
 */

#include "HttpResponse.hpp"
#include "../../3rdParty/zlib/ZLib.hpp"

using namespace std;

HttpResponse::HttpResponse(unsigned short statusCode, String content) {
    this->status = statusCode;
    this->content = content;
}

HttpResponse::~HttpResponse() {
}

String HttpResponse::toString() {
    string returnString;

    returnString.append(Core::ServerProtocol + " " + this->getStatusString() + ENDL);
    returnString.append("Host: " + Core::ServerAddress + ":" + to_string(Core::ServerPort) + ENDL);
    returnString.append("Server: " + Core::ServerName + ENDL);
    returnString.append("X-Powered-By: " + Core::ServerName + ENDL);
    returnString.append("Connection: close" + ENDL);
    returnString.append("Content-Type: " + this->type + "; charset=UTF-8" + ENDL);
    returnString.append("Content-Length: " + to_string(this->content.size()) + ENDL);

    //Para que não seja compactado o conteúdo de arquivos (previamente compactado pelo módulo FileLibrary).
    if (this->compressOutput) {
        //returnString.append("Transfer-Encoding: gzip" + ENDL);

        this->content = ZLib::compress_string(this->content);
        returnString.append("Content-Encoding: deflate" + ENDL);
    }

    returnString.append(ENDL);
    returnString.append(this->content);

    //returnString.append("Vary: Accept-Encoding, Cookie, User-Agent" + ENDL);
    //returnString.append("Date: Thu, 08 Oct 2015 16:42:10 GMT" + ENDL);
    //returnString.append("Pragma: public" + ENDL);
    //returnString.append("Expires: Sat, 28 Nov 2009 05:36:25 GMT" + ENDL);
    //returnString.append("Last-Modified: Sat, 28 Nov 2009 03:50:37 GMT" + ENDL);
    //returnString.append("X-Pingback: http://net.tutsplus.com/xmlrpc.php" + ENDL);

    return returnString;
}

String HttpResponse::getStatusString() {
    switch (this->status) {
        case 200:
            return "200 OK";

        case 401:
            return "401 Forbidden";

        case 404:
            return "404 Not Found";

        case 500:
            return "500 Internal Server Error";

        case 503:
            return "503 Service Unavailable";

        default:
            return to_string(this->status);
    }
}
