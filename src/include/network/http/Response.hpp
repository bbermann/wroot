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

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include <include/type/String.hpp>
#include <include/network/http/KeyValuePair.hpp>

/// A reply to be sent to a client.
struct Response
{
    int httpVersionMajor = 1;
    int httpVersionMinor = 1;

    /// The status of the response.
    enum StatusType
    {
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
    StringMap headers;

    std::string getHeader(const std::string &key) {
        return headers[key];
    }

    void setHeader(const std::string &key, const std::string &value) {
        headers[key] = value;
    }

    /// The content to be sent in the response.
    std::string content;

    /// Convert the response into a vector of buffers. The buffers do not own the
    /// underlying memory blocks, therefore the response object must remain valid and
    /// not be changed until the write operation has completed.
    std::vector<boost::asio::const_buffer> toBuffers();

    std::string serialize() const;

    /// Get a stock response.
    static Response stockResponse(StatusType status);

    static Response unserialize(std::string &serialized);
};