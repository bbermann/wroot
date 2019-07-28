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

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP

#include "Core.hpp"

class HttpResponse {
public:
    HttpResponse();

    virtual ~HttpResponse();

    String type;
    String content;
    bool compressOutput;
    unsigned short status;

    String toString();

private:
    String getStatusString();
};

#endif /* HTTPRESPONSE_HPP */

