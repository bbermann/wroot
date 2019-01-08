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
	this->request = request;
}

HttpRequest CustomLibrary::getHttpRequest()
{
	return request;
}

HttpResponse CustomLibrary::getResponse() 
{
	HttpResponse response;
	response.content = this->toString();
	response.status = this->statusCode;
	response.type = this->responseType;
	response.compressOutput = this->compressedOutput;
	return response;
}

String CustomLibrary::getRequestUrl()
{
	return request.getUrl();
}