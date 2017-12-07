/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HttpContent.cpp
 * Author: bbermann
 * 
 * Created on 7 de Fevereiro de 2016, 00:57
 */

#include "HttpResponse.hpp"
#include "../../3rdParty/zlib/ZLib.hpp"

using namespace std;

HttpResponse::HttpResponse()
{
}

HttpResponse::~HttpResponse()
{
}

String HttpResponse::toString()
{
    string returnString;

    returnString.append(Core::ServerProtocol + " " + getStatusString() + ENDL);
    returnString.append("Host: " + Core::ServerAddress + ":" + to_string(Core::ServerPort) + ENDL);
    returnString.append("Server: " + Core::ServerName + ENDL);
    returnString.append("X-Powered-By: " + Core::ServerName + ENDL);
    returnString.append("Connection: keep-alive" + ENDL);
    returnString.append("Content-Type: " + type + "; charset=UTF-8" + ENDL);
    returnString.append("Content-Length: " + to_string(content.size()) + ENDL);

	//Para que n�o seja compactado o conte�do de arquivos (previamente compactado pelo m�dulo FileLibrary).
	if (compressOutput)
	{
		content = ZLib::compress_string(content);

		//returnString.append("Transfer-Encoding: gzip" + ENDL);
		returnString.append("Content-Encoding: deflate" + ENDL);
	}

	returnString.append(ENDL);
	returnString.append(content);

    //TODO Implementar
    /*if(Core::UseBrowserCache)
    {
        returnString.append("Etag: \"pub1259380237;gz\"" + ENDL);
    }
    else
    {
        returnString.append("Cache-Control: no-cache" + ENDL);
    }*/

	//returnString.append("Vary: Accept-Encoding, Cookie, User-Agent" + ENDL);
	//returnString.append("Date: Thu, 08 Oct 2015 16:42:10 GMT" + ENDL);
	//returnString.append("Pragma: public" + ENDL);
	//returnString.append("Expires: Sat, 28 Nov 2009 05:36:25 GMT" + ENDL);
	//returnString.append("Last-Modified: Sat, 28 Nov 2009 03:50:37 GMT" + ENDL);
	//returnString.append("X-Pingback: http://net.tutsplus.com/xmlrpc.php" + ENDL);

	return returnString;
}

String HttpResponse::getStatusString()
{
    switch (status)
    {
    case 200:
        return "200 OK";

    default:
        return to_string(status);
    }
}
