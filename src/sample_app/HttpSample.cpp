#include <sample_app/HttpSample.hpp>
#include <include/network/http/Request.hpp>

HttpSample::HttpSample(const Request &request) : CustomLibrary(request) {
}

HttpSample::~HttpSample() {
}

String HttpSample::toString() {
    return "<h1>It works!</h1>";
}