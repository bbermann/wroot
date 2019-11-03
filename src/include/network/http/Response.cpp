#include "Response.hpp"
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <include/core/Core.hpp>
#include <memory>

namespace StatusStrings {
    const std::string Ok = "200 OK";
    const std::string Created = "201 Created";
    const std::string Accepted = "202 Accepted";
    const std::string NoContent = "204 No Content";
    const std::string MultipleChoices = "300 Multiple Choices";
    const std::string MovedPermanently = "301 Moved Permanently";
    const std::string MovedTemporarily = "302 Moved Temporarily";
    const std::string NotModified = "304 Not Modified";
    const std::string BadRequest = "400 Bad Request";
    const std::string Unauthorized = "401 Unauthorized";
    const std::string Forbidden = "403 Forbidden";
    const std::string NotFound = "404 Not Found";
    const std::string InternalServerError = "500 Internal Server Error";
    const std::string NotImplemented = "501 Not Implemented";
    const std::string BadGateway = "502 Bad Gateway";
    const std::string ServiceUnavailable = "503 Service Unavailable";

    boost::asio::const_buffer toBuffer(Response::StatusType status) {
        switch (status) {
            case Response::Ok:
                return boost::asio::buffer(Ok);
            case Response::Created:
                return boost::asio::buffer(Created);
            case Response::Accepted:
                return boost::asio::buffer(Accepted);
            case Response::NoContent:
                return boost::asio::buffer(NoContent);
            case Response::MultipleChoices:
                return boost::asio::buffer(MultipleChoices);
            case Response::MovedPermanently:
                return boost::asio::buffer(MovedPermanently);
            case Response::MovedTemporarily:
                return boost::asio::buffer(MovedTemporarily);
            case Response::NotModified:
                return boost::asio::buffer(NotModified);
            case Response::BadRequest:
                return boost::asio::buffer(BadRequest);
            case Response::Unauthorized:
                return boost::asio::buffer(Unauthorized);
            case Response::Forbidden:
                return boost::asio::buffer(Forbidden);
            case Response::NotFound:
                return boost::asio::buffer(NotFound);
            case Response::InternalServerError:
                return boost::asio::buffer(InternalServerError);
            case Response::NotImplemented:
                return boost::asio::buffer(NotImplemented);
            case Response::BadGateway:
                return boost::asio::buffer(BadGateway);
            case Response::ServiceUnavailable:
                return boost::asio::buffer(ServiceUnavailable);
            default:
                return boost::asio::buffer(InternalServerError);
        }
    }
} // namespace StatusStrings

namespace MiscStrings {
    const char NameValueSeparator[] = {':', ' '};
    const char CRLF[] = {'\r', '\n'};
} // namespace MiscStrings

std::vector<boost::asio::const_buffer> Response::toBuffers() {
    std::vector<boost::asio::const_buffer> buffers;

    using namespace std;

    const shared_ptr<string> version = make_shared<string>("HTTP/");
    version->append(to_string(httpVersionMajor));
    version->append(".");
    version->append(std::to_string(httpVersionMinor));
    version->append(" ");

    buffers.emplace_back(boost::asio::buffer(*version));

    buffers.push_back(StatusStrings::toBuffer(status));
    buffers.push_back(boost::asio::buffer(MiscStrings::CRLF));

    for (const auto &[key, value] : headers) {
        buffers.push_back(boost::asio::buffer(key));
        buffers.push_back(boost::asio::buffer(MiscStrings::NameValueSeparator));
        buffers.push_back(boost::asio::buffer(value));
        buffers.push_back(boost::asio::buffer(MiscStrings::CRLF));
    }

    buffers.push_back(boost::asio::buffer(MiscStrings::CRLF));

    buffers.emplace_back(boost::asio::buffer(content));

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
    response.headers["Content-Length"] = std::to_string(response.content.size());
    response.headers["Content-Type"] = "text/html";
    return response;
}

std::string Response::serialize() const {
    using json = nlohmann::json;

    json serialized;
    serialized["content"] = this->content;
    serialized["status"] = this->status;

    for (auto[key, value] : this->headers) {
        serialized["headers"][key] = value;
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
            response.headers[it.key()] = it.value();
        }
    }

    if (responseData.find("content") != responseData.end()) {
        response.content = responseData["content"];
    }

    return response;
}
