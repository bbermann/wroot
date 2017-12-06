#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "../type/String.hpp"

class HttpRequest
{
public:
    HttpRequest();
	HttpRequest(String request);
	virtual ~HttpRequest();

    String getHttpMethod();
    String getUrl();

protected:
	void setHttpMethod(String method);
	void setUrl(String url);

private:
	String url_;
	String http_method_;
};

#endif //HTTPREQUEST_HPP