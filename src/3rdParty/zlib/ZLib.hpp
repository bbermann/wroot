/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZLib.hpp
 * Author: bbermann
 *
 * Created on 6 de Março de 2016, 23:43
 */

#ifndef ZLIB_HPP
#define ZLIB_HPP

#define Z_BEST_COMPRESSION 9

#include "../../include/core/Core.hpp"

class ZLib {
public:
    ZLib();
    ZLib(const ZLib& orig);
    virtual ~ZLib();
    
    static std::string compress_string(const std::string& str, int compressionlevel = Z_BEST_COMPRESSION);
    static std::string decompress_string(const std::string& str);
private:

};

#endif /* ZLIB_HPP */

