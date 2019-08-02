#include "CustomLibrary.hpp"
#include "../exceptions/http/NotFound.hpp"
#include "../exceptions/http/InternalServerError.hpp"
#include "../exceptions/http/Unauthorized.hpp"
#include "../exceptions/http/Forbidden.hpp"

using namespace std;

CustomLibrary::CustomLibrary() {
}

CustomLibrary::~CustomLibrary() {
}

void CustomLibrary::setHttpRequest(HttpRequest request) {
    this->request = request;
}

HttpRequest CustomLibrary::getHttpRequest() {
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