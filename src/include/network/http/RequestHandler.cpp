//
// Created by bbermann on 10/10/2019.
//

#include "RequestHandler.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include "MimeTypes.hpp"
#include "Response.hpp"
#include "Request.hpp"

RequestHandler::RequestHandler(const std::string &documentRoot) : documentRoot_(documentRoot) {
}

void RequestHandler::handle(const Request &request, Response &response) {
    // Decode url to path.
    std::string request_path;

    if (!this->urlDecode(request.uri, request_path)) {
        response = Response::stockResponse(Response::BadRequest);
        return;
    }

    // Request path must be absolute and not contain "..".
    if (request_path.empty() || request_path[0] != '/' || request_path.find("..") != std::string::npos) {
        response = Response::stockResponse(Response::BadRequest);
        return;
    }

    // If path ends in slash (i.e. is a directory) then add "index.html".
    if (request_path[request_path.size() - 1] == '/') {
        request_path += "index.html";
    }

    // Determine the file extension.
    std::size_t last_slash_pos = request_path.find_last_of("/");
    std::size_t last_dot_pos = request_path.find_last_of(".");
    std::string extension = "";
    if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
        extension = request_path.substr(last_dot_pos + 1);
    }

    // Open the file to send back.
    std::string full_path = this->documentRoot_ + request_path;
    std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
    if (!is) {
        response = Response::stockResponse(Response::NotFound);
        return;
    }

    // Fill out the reply to be sent to the client.
    response.status = Response::Ok;
    char buf[512];

    while (is.read(buf, sizeof(buf)).gcount() > 0) {
        response.content.append(buf, is.gcount());
    }

    response.headers.resize(2);
    response.headers[0].name = "Content-Length";
    response.headers[0].value = std::to_string(response.content.size());
    response.headers[1].name = "Content-Type";
    response.headers[1].value = MimeTypes::extensionToType(extension);
}

bool RequestHandler::urlDecode(const std::string &in, std::string &out) {
    out.clear();
    out.reserve(in.size());

    for (std::size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '%') {
            if (i + 3 <= in.size()) {
                int value = 0;
                std::istringstream is(in.substr(i + 1, 2));

                if (is >> std::hex >> value) {
                    out += static_cast<char>(value);
                    i += 2;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else if (in[i] == '+') {
            out += ' ';
        } else {
            out += in[i];
        }
    }

    return true;
}