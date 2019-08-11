#include "HttpRequest.hpp"
#include "Core.hpp"

HttpRequest::HttpRequest() {
    this->request_ = "";
    this->isValid_ = false;
}

HttpRequest::HttpRequest(const String &request, const String &ipAddress) {
    this->request_ = request;
    this->set("REMOTE_ADDR", ipAddress);

    process();

    //Core::debugLn(this->toString());

    this->isValid_ = true;
}

HttpRequest::~HttpRequest() = default;

bool HttpRequest::isValid() const {
    return this->isValid_;
}

String HttpRequest::get(const String &key) const {
    return "";//this->data_.at(key);
}

String HttpRequest::getHttpMethod() const {
    return this->get("HTTP_METHOD");
}

String HttpRequest::getUrl() const {
    return this->get("REQUEST_URI");
}

StringMap HttpRequest::getQuery() const {
    return this->query_;
}

String HttpRequest::toString() const {
    String output;

    for (auto pair : this->data_) {
        output.append(pair.first + ": " + pair.second + ENDL);
    }

    return output;
}

void HttpRequest::process() {
    bool firstRow = true;

    StringList rows = this->request_.explode(ENDL);

    for (String row : rows) {
        if (row.trim().empty()) {
            continue;
        }

        if (firstRow) {
            firstRow = false;

            // # GET /home HTTP/1.1
            Core::debugLn(row);

            StringList explodedRequest = row.explode(" ");
            int explodedSize = explodedRequest.size();

            if (explodedSize > 1) {
                this->setHttpMethod(explodedRequest.at(0));

                if (explodedSize > 2) {
                    String fullUrl = explodedRequest.at(1);
                    StringList parts = fullUrl.explode("?");

                    String url = parts.at(0);
                    String queryString = parts.size() > 1 ? parts.at(parts.size() - 1) : "";

                    this->setUrl(url);
                    this->setQuery(queryString);
                }
            } else {
                this->setHttpMethod("GET");
                this->setUrl("");
                this->setQuery("");
            }
        } else {
            StringList keyVal = row.explode(": ");

            if (keyVal.size() == 2) {
                String key = keyVal.at(0);
                String value = keyVal.at(1);

                this->set(key, value);
            }
        }
    }
}

void HttpRequest::set(const String &key, const String &value) {
    this->data_[key] = value;
}

void HttpRequest::setUrl(String url) {
    if (url.contains("?")) {
        url = url.explode("?").at(0);
    }

    this->set("REQUEST_URI", url);
}

void HttpRequest::setHttpMethod(String method) {
    if (method != "GET" && method != "POST" &&
        method != "DELETE" && method != "PUT") {
        method = "GET";
    }

    this->set("HTTP_METHOD", method);
}

void HttpRequest::setQueryParam(const String &key, const String &value) {
    this->query_[key] = value;
}

void HttpRequest::setQuery(const String &queryString) {
    this->set("QUERY_STRING", queryString);

    StringList data = queryString.explode("&");

    for (String row : data) {
        StringList keyVal = row.explode("=");

        if (keyVal.size() == 2) {
            this->setQueryParam(keyVal.at(0), keyVal.at(1));
        }
    }
}