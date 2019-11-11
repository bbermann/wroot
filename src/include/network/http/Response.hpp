/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Response.hpp
 * Author: bbermann
 *
 * Created on 7 de Fevereiro de 2016, 00:57
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <boost/asio.hpp>
#include <include/core/Core.hpp>
#include <include/type/String.hpp>
#include "Request.hpp"

struct Request;

/// A reply to be sent to a client.
struct Response {
    int httpVersionMajor = 1;
    int httpVersionMinor = 1;

    /// The status of the response.
    enum StatusType {
        Ok = 200,
        Created = 201,
        Accepted = 202,
        NoContent = 204,
        MultipleChoices = 300,
        MovedPermanently = 301,
        MovedTemporarily = 302,
        NotModified = 304,
        BadRequest = 400,
        Unauthorized = 401,
        Forbidden = 403,
        NotFound = 404,
        InternalServerError = 500,
        NotImplemented = 501,
        BadGateway = 502,
        ServiceUnavailable = 503
    } status;

    /// The headers to be included in the response.
    std::multimap<std::string, std::string> headers;

    /// The content to be sent in the response.
    std::string content;

    /// Convert the response into a vector of buffers. The buffers do not own the
    /// underlying memory blocks, therefore the response object must remain valid and
    /// not be changed until the write operation has completed.
    std::vector<boost::asio::const_buffer> toBuffers();

    std::string getHeader(const std::string &key) {
        auto found = this->headers.find(key);

        if (found == this->headers.end()) {
            return "";
        }

        return found->second;
    }

    std::vector<std::string> getHeaders(const std::string &key) {
        auto found = this->headers.find(key);

        if (found == this->headers.end()) {
            return {};
        }

        std::vector<std::string> vec;

        do {
            vec.push_back(found->second);

            found++;
        } while (found != this->headers.end());

        return vec;
    }

    void setHeader(const std::string &key, const std::string &value) {
        this->headers.insert({key, value});
    }

    void bind(const Request &request) {
        this->httpVersionMajor = request.httpVersionMajor;
        this->httpVersionMinor = request.httpVersionMinor;

        // If there's no Connection header set
        if (this->headers.find("Connection") == this->headers.end()) {
            // If Connection header is set on request
            if (request.headers.find("Connection") != request.headers.end() &&
                !request.headers.find("Connection")->second.empty()) {
                // Reply as requested
                this->headers.insert({"Connection", request.getHeader("Connection")});
            } else {
                // Otherwise, set close as default
                this->headers.insert({"Connection", "close"});
            }
        }
    }

    std::string serialize() const;

    static Response unserialize(std::string &serialized);

    /// Get a stock response.
    static Response stockResponse(StatusType status);
};