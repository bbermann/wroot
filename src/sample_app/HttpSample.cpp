#include <sample_app/HttpSample.hpp>
#include <include/network/http/Request.hpp>

HttpSample::HttpSample(const Request &request) : CustomLibrary(request) {
}

HttpSample::~HttpSample() {
}

void HttpSample::handle(Response &response) {
    response.status = Response::Ok;
    response.content = "<h1>It works!</h1>";
}