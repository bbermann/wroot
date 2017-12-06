#include "CustomLibrary.hpp"

using namespace std;

CustomLibrary::CustomLibrary() 
{
}

CustomLibrary::~CustomLibrary() 
{
}

void CustomLibrary::setHttpRequest(HttpRequest request)
{
	request_ = request;
}

HttpRequest CustomLibrary::getHttpRequest()
{
	return request_;
}

HttpResponse CustomLibrary::getResponse() 
{
	HttpResponse response;
	response.content = this->toString();
	response.status = this->statusCode;
	response.type = this->responseType;
	response.compressOutput = this->useCompressedOutput;
	return response;
}

String CustomLibrary::getRequestUrl()
{
	return request_.getUrl();
}