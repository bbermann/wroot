//
// Created by bbermann on 10/10/2019.
//

#pragma once

#include <string>

struct Response;
struct Request;

/// The common handler for all incoming requests.
class RequestHandler {
public:
    RequestHandler(const RequestHandler &) = delete;
    RequestHandler &operator=(const RequestHandler &) = delete;

    /// Construct with a directory containing files to be served.
    explicit RequestHandler();

    /// Handle a request and produce a reply.
    void handle(const Request &request, Response &response);

    void finishResponse(const Request &request, Response &response);
};