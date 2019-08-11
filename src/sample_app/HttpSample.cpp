#include <sample_app/HttpSample.hpp>
#include <include/core/HttpRequest.hpp>

HttpSample::HttpSample(const HttpRequest &request) : CustomLibrary(request) {
}

HttpSample::~HttpSample() {
}

String HttpSample::toString() {
    return "<h1>It works!</h1>";
}