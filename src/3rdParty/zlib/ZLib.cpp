/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ZLib.cpp
 * Author: bbermann
 * 
 * Created on 6 de Março de 2016, 23:43
 */

#include "ZLib.hpp"

using namespace std;

// Copyright 2007 Timo Bingmann <tb@panthema.net>
// Distributed under the Boost Software License, Version 1.0.
// (See http://www.boost.org/LICENSE_1_0.txt)

#include <stdexcept>
#include <iomanip>
#include <sstream>
#include "include/zlib.h"

ZLib::ZLib()
{
}

ZLib::ZLib(const ZLib& orig)
{
}

ZLib::~ZLib()
{
}

/** Compress a STL string using zlib with given compression level and return
 * the binary data. */
string ZLib::compress_string(const std::string& str, int compressionlevel)
{
    z_stream zs; // z_stream is zlib's control structure
    memset(&zs, 0, sizeof (zs));

    if (deflateInit(&zs, compressionlevel) != Z_OK)
        throw (std::runtime_error("deflateInit failed while compressing."));

    zs.next_in = (Bytef*) str.data();
    zs.avail_in = str.size(); // set the z_stream's input

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // retrieve the compressed bytes blockwise
    do
    {
        zs.next_out = reinterpret_cast<Bytef*> (outbuffer);
        zs.avail_out = sizeof (outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out)
        {
            // append the block to the output string
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }
    }
    while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END)
    { // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw (std::runtime_error(oss.str()));
    }

    return outstring;
}

/** Decompress an STL string using zlib and return the original data. */
string ZLib::decompress_string(const string& str)
{
    z_stream zs; // z_stream is zlib's control structure
    memset(&zs, 0, sizeof (zs));

    if (inflateInit(&zs) != Z_OK)
        throw (std::runtime_error("inflateInit failed while decompressing."));

    zs.next_in = (Bytef*) str.data();
    zs.avail_in = str.size();

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // get the decompressed bytes blockwise using repeated calls to inflate
    do
    {
        zs.next_out = reinterpret_cast<Bytef*> (outbuffer);
        zs.avail_out = sizeof (outbuffer);

        ret = inflate(&zs, 0);

        if (outstring.size() < zs.total_out)
        {
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }

    }
    while (ret == Z_OK);

    inflateEnd(&zs);

    if (ret != Z_STREAM_END)
    { // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") "
                << zs.msg;
        throw (std::runtime_error(oss.str()));
    }

    return outstring;
}