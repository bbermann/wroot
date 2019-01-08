#ifndef CUSTOMLIBRARY_HPP
#define CUSTOMLIBRARY_HPP

#include "../core/Core.hpp"
#include "../core/HttpResponse.hpp"
#include "../core/HttpRequest.hpp"

class CustomLibrary
{
public:
    CustomLibrary();
    virtual ~CustomLibrary();

    ///<summary>Function is a container to the url name and the function pointer.</summary>
    typedef std::pair<std::string, void(*)() > Function;

    ///<summary>FunctionList will be a vector of Function.</summary>
    typedef std::vector<std::pair<std::string, void(*)()> > FunctionList;

    ///<summary>
    ///Add a new function to be used calling by url passed to first parameter and 
    ///processed by the second parameter method.
    ///</summary>
    void addHttpFunction(std::string& _function, void(*_process)());

    ///<summary>HttpRequest</summary>
    void setHttpRequest(HttpRequest request);
    HttpRequest getHttpRequest();

    ///<summary>Return url from HttpRequest</summary>
    String getRequestUrl();

    ///<summary>If no library was set before, is_empty= true</summary>
    bool is_empty = false;

	///<summary>Abstract method implemented in children classes</summary>
	virtual String toString() = 0;

	///<summary>Get the HttpResponse object passed to browser.</summary>
	virtual HttpResponse getResponse();

protected:
	bool compressedOutput = Core::CompressedOutput;
	unsigned short statusCode = 200;
    HttpRequest request;
	String responseType = "text/plain";
};


#endif