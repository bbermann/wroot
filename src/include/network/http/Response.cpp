/*
String Response::toString() {
    String returnString;

    returnString.append(Core::ServerProtocol + " " + this->getStatusString() + ENDL);
    returnString.append("Host: " + Core::ServerAddress + ENDL);
    returnString.append("Server: " + Core::ServerName + ENDL);
    returnString.append("X-Powered-By: " + Core::ServerName + ENDL);
    returnString.append("Connection: close" + ENDL);
    returnString.append("Content-Type: " + this->type + "; charset=UTF-8" + ENDL);
    returnString.append("Content-Length: " + std::to_string(this->content.size()) + ENDL);

    // Insert the extra headers from external sources.
    for (const auto &item : this->data_) {
        returnString.append(item.first + ": " + item.second + ENDL);
    }

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
 */

#include "Response.hpp"
#include <string>
#include <nlohmann/json/src/json.hpp>

namespace StatusStrings {
    const std::string Ok = "HTTP/1.0 200 OK\r\n";
    const std::string Created = "HTTP/1.0 201 Created\r\n";
    const std::string Accepted = "HTTP/1.0 202 Accepted\r\n";
    const std::string NoContent = "HTTP/1.0 204 No Content\r\n";
    const std::string MultipleChoices = "HTTP/1.0 300 Multiple Choices\r\n";
    const std::string MovedPermanently = "HTTP/1.0 301 Moved Permanently\r\n";
    const std::string MovedTemporarily = "HTTP/1.0 302 Moved Temporarily\r\n";
    const std::string NotModified = "HTTP/1.0 304 Not Modified\r\n";
    const std::string BadRequest = "HTTP/1.0 400 Bad Request\r\n";
    const std::string Unauthorized = "HTTP/1.0 401 Unauthorized\r\n";
    const std::string Forbidden = "HTTP/1.0 403 Forbidden\r\n";
    const std::string NotFound = "HTTP/1.0 404 Not Found\r\n";
    const std::string InternalServerError = "HTTP/1.0 500 Internal Server Error\r\n";
    const std::string NotImplemented = "HTTP/1.0 501 Not Implemented\r\n";
    const std::string BadGateway = "HTTP/1.0 502 Bad Gateway\r\n";
    const std::string ServiceUnavailable = "HTTP/1.0 503 Service Unavailable\r\n";

    asio::const_buffer toBuffer(Response::StatusType status) {
        switch (status) {
            case Response::Ok:
                return asio::buffer(Ok);
            case Response::Created:
                return asio::buffer(Created);
            case Response::Accepted:
                return asio::buffer(Accepted);
            case Response::NoContent:
                return asio::buffer(NoContent);
            case Response::MultipleChoices:
                return asio::buffer(MultipleChoices);
            case Response::MovedPermanently:
                return asio::buffer(MovedPermanently);
            case Response::MovedTemporarily:
                return asio::buffer(MovedTemporarily);
            case Response::NotModified:
                return asio::buffer(NotModified);
            case Response::BadRequest:
                return asio::buffer(BadRequest);
            case Response::Unauthorized:
                return asio::buffer(Unauthorized);
            case Response::Forbidden:
                return asio::buffer(Forbidden);
            case Response::NotFound:
                return asio::buffer(NotFound);
            case Response::InternalServerError:
                return asio::buffer(InternalServerError);
            case Response::NotImplemented:
                return asio::buffer(NotImplemented);
            case Response::BadGateway:
                return asio::buffer(BadGateway);
            case Response::ServiceUnavailable:
                return asio::buffer(ServiceUnavailable);
            default:
                return asio::buffer(InternalServerError);
        }
    }
} // namespace StatusStrings

namespace MiscStrings {
    const char NameValueSeparator[] = {':', ' '};
    const char CRLF[] = {'\r', '\n'};
} // namespace MiscStrings

std::vector<asio::const_buffer> Response::toBuffers() {
    std::vector<asio::const_buffer> buffers;

    buffers.push_back(StatusStrings::toBuffer(status));

    for (std::size_t i = 0; i < headers.size(); ++i) {
        Header &header = headers[i];

        // key:value\r\n
        buffers.push_back(asio::buffer(header.name));
        buffers.push_back(asio::buffer(MiscStrings::NameValueSeparator));
        buffers.push_back(asio::buffer(header.value));
        buffers.push_back(asio::buffer(MiscStrings::CRLF));
    }

    buffers.push_back(asio::buffer(MiscStrings::CRLF));
    buffers.push_back(asio::buffer(content));

    return buffers;
}

namespace StockResponses {
    const char Ok[] = "";
    const char Created[] =
            "<html>"
            "<head><title>Created</title></head>"
            "<body><h1>201 Created</h1></body>"
            "</html>";
    const char Accepted[] =
            "<html>"
            "<head><title>Accepted</title></head>"
            "<body><h1>202 Accepted</h1></body>"
            "</html>";
    const char NoContent[] =
            "<html>"
            "<head><title>No Content</title></head>"
            "<body><h1>204 Content</h1></body>"
            "</html>";
    const char MultipleChoices[] =
            "<html>"
            "<head><title>Multiple Choices</title></head>"
            "<body><h1>300 Multiple Choices</h1></body>"
            "</html>";
    const char MovedPermanently[] =
            "<html>"
            "<head><title>Moved Permanently</title></head>"
            "<body><h1>301 Moved Permanently</h1></body>"
            "</html>";
    const char MovedTemporarily[] =
            "<html>"
            "<head><title>Moved Temporarily</title></head>"
            "<body><h1>302 Moved Temporarily</h1></body>"
            "</html>";
    const char NotModified[] =
            "<html>"
            "<head><title>Not Modified</title></head>"
            "<body><h1>304 Not Modified</h1></body>"
            "</html>";
    const char BadRequest[] =
            "<html>"
            "<head><title>Bad Request</title></head>"
            "<body><h1>400 Bad Request</h1></body>"
            "</html>";
    const char Unauthorized[] =
            "<html>"
            "<head><title>Unauthorized</title></head>"
            "<body><h1>401 Unauthorized</h1></body>"
            "</html>";
    const char Forbidden[] =
            "<html>"
            "<head><title>Forbidden</title></head>"
            "<body><h1>403 Forbidden</h1></body>"
            "</html>";
    const char NotFound[] =
            "<html>"
            "<head><title>Not Found</title></head>"
            "<body><h1>404 Not Found</h1></body>"
            "</html>";
    const char InternalServerError[] =
            "<html>"
            "<head><title>Internal Server Error</title></head>"
            "<body><h1>500 Internal Server Error</h1></body>"
            "</html>";
    const char NotImplemented[] =
            "<html>"
            "<head><title>Not Implemented</title></head>"
            "<body><h1>501 Not Implemented</h1></body>"
            "</html>";
    const char BadGateway[] =
            "<html>"
            "<head><title>Bad Gateway</title></head>"
            "<body><h1>502 Bad Gateway</h1></body>"
            "</html>";
    const char ServiceUnavailable[] =
            "<html>"
            "<head><title>Service Unavailable</title></head>"
            "<body><h1>503 Service Unavailable</h1></body>"
            "</html>";

    std::string toString(Response::StatusType status) {
        switch (status) {
            case Response::Ok:
                return Ok;
            case Response::Created:
                return Created;
            case Response::Accepted:
                return Accepted;
            case Response::NoContent:
                return NoContent;
            case Response::MultipleChoices:
                return MultipleChoices;
            case Response::MovedPermanently:
                return MovedPermanently;
            case Response::MovedTemporarily:
                return MovedTemporarily;
            case Response::NotModified:
                return NotModified;
            case Response::BadRequest:
                return BadRequest;
            case Response::Unauthorized:
                return Unauthorized;
            case Response::Forbidden:
                return Forbidden;
            case Response::NotFound:
                return NotFound;
            case Response::InternalServerError:
                return InternalServerError;
            case Response::NotImplemented:
                return NotImplemented;
            case Response::BadGateway:
                return BadGateway;
            case Response::ServiceUnavailable:
                return ServiceUnavailable;
            default:
                return InternalServerError;
        }
    }

} // namespace StockResponses

Response Response::stockResponse(Response::StatusType status) {
    Response response;
    response.status = status;
    response.content = StockResponses::toString(status);
    response.headers.resize(2);
    response.headers[0].name = "Content-Length";
    response.headers[0].value = std::to_string(response.content.size());
    response.headers[1].name = "Content-Type";
    response.headers[1].value = "text/html";
    return response;
}

#include <iostream>

std::string Response::serialize() {
    using json = nlohmann::json;

    json serialized;
    serialized["content"] = this->content;
    serialized["status"] = this->status;

    for (auto header : this->headers) {
        serialized["headers"][header.name] = header.value;
    }

    return serialized.dump();
}

Response Response::unserialize(std::string &serialized) {
    using json = nlohmann::json;

    json responseData = json::parse(serialized);

    Response response;
    response.status = responseData["status"];

    if (responseData.find("headers") != responseData.end()) {
        for (auto it = responseData["headers"].begin(); it != responseData["headers"].end(); ++it) {
            Header header;
            header.name = it.key();
            header.value = it.value();

            response.headers.push_back(header);
        }
    }

    if (responseData.find("content") != responseData.end()) {
        response.content = responseData["content"];
    }

    return response;
}
