//
// Created by bbermann on 10/10/2019.
//

#pragma once

#include <atomic>
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

protected:
    static std::atomic<unsigned long long> RequestCount;
    static std::atomic<unsigned long long> ResponseCount;

    void processResponse(const Request &request, Response &response) const;

    void finishResponse(const Request &request, Response &response);
};