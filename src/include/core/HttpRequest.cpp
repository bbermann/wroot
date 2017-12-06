#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
{
	setHttpMethod("GET");
	setUrl("");
}

HttpRequest::~HttpRequest()
{
}

HttpRequest::HttpRequest(String request)
{
	StringList explodedRequest = request.explode(" ");

    int explodedSize = explodedRequest.size();

    if (explodedSize > 0)
    {
        setHttpMethod(explodedRequest.at(0));

        if (explodedSize > 1)
        {
            setUrl(explodedRequest.at(1));
        }
    }
}

String HttpRequest::getHttpMethod()
{
	return http_method_;
}

String HttpRequest::getUrl()
{
	return url_;
}

void HttpRequest::setUrl(String url)
{
	url_ = url;
}

void HttpRequest::setHttpMethod(String method) 
{
	if (method == "GET" || method == "POST" ||
		method == "DELETE" || method == "PUT") 
	{
		http_method_ = method;
	} 
	else 
	{
		http_method_ = "GET";
	}
}