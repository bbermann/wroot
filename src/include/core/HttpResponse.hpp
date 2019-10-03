/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HttpResponse.hpp
 * Author: bbermann
 *
 * Created on 7 de Fevereiro de 2016, 00:57
 */

#pragma once

#include <include/core/Core.hpp>
#include <include/core/HttpRequest.hpp>

class HttpResponse {
public:
    explicit HttpResponse(unsigned short statusCode = 200, const String &content = "");

    explicit HttpResponse(const HttpRequest &request);

    virtual ~HttpResponse();

    String type = "text/plain";
    String content = "";
    bool compressOutput = Core::CompressedOutput;
    unsigned short status = 200;

    String toString();

private:
    String getStatusString();
};
