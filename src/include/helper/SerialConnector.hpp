/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SerialConnector.hpp
 * Author: bbermann
 *
 * Created on 12 de Novembro de 2015, 20:00
 */

#ifndef WINDOWS

#ifndef SERIALCONNECTOR_HPP
#define SERIALCONNECTOR_HPP

#include "../type/String.hpp"

class SerialConnector {
public:

    SerialConnector(const char* portname);
    SerialConnector(const SerialConnector& orig);
    virtual ~SerialConnector();

    void start();
    void stop();
    void serialWrite(const char* str);

private:
    
    int setInterfaceAttribs(int fd, int speed, int parity);
    void setBlocking(int fd, int should_block);

    int fd;
    String portname_;
};

#endif /* SERIALCONNECTOR_HPP */


#endif /* WINDOWS */