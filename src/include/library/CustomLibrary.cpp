#include <include/library/CustomLibrary.hpp>
#include <include/exceptions/http/response/NotFound.hpp>
#include <include/exceptions/http/response/InternalServerError.hpp>
#include <include/exceptions/http/response/Unauthorized.hpp>
#include <include/exceptions/http/response/Forbidden.hpp>

using namespace std;

CustomLibrary::CustomLibrary(const HttpRequest &request) : request(request) {

}

CustomLibrary::~CustomLibrary() = default;

const HttpRequest& CustomLibrary::getHttpRequest() {
    return request;
}

String CustomLibrary::toString() {
    return "";
}

HttpResponse CustomLibrary::getResponse() {
    HttpResponse response(this->statusCode);
    response.type = this->responseType;
    response.compressOutput = this->compressedOutput;

    try {
        response.content = this->toString();
    } catch (const NotFound &exception) {
        response.status = 404;
    } catch (const Unauthorized &exception) {
        response.status = 401;
    } catch (const Forbidden &exception) {
        response.status = 403;
    } catch (const InternalServerError &exception) {
        response.status = 500;
        response.content = exception.what();
    }

    return response;
}

String CustomLibrary::getRequestUrl() {
    return request.getUrl();
}