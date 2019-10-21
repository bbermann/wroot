#include <include/library/CustomLibrary.hpp>
#include <include/exceptions/http/response/NotFound.hpp>
#include <include/exceptions/http/response/InternalServerError.hpp>
#include <include/exceptions/http/response/Unauthorized.hpp>
#include <include/exceptions/http/response/Forbidden.hpp>

using namespace std;

CustomLibrary::CustomLibrary(const Request &request)
: request(request) {

}

CustomLibrary::~CustomLibrary() = default;

const Request &CustomLibrary::getHttpRequest() {
    return request;
}

bool CustomLibrary::urlDecode(const std::string &in, std::string &out) {
    out.clear();
    out.reserve(in.size());

    for (std::size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '%') {
            if (i + 3 <= in.size()) {
                int value = 0;
                std::istringstream is(in.substr(i + 1, 2));

                if (is >> std::hex >> value) {
                    out += static_cast<char>(value);
                    i += 2;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else if (in[i] == '+') {
            out += ' ';
        } else {
            out += in[i];
        }
    }

    return true;
}