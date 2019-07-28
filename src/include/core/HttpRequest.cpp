#include "HttpRequest.hpp"
#include "Core.hpp"

HttpRequest::HttpRequest() {
    this->request_ = "";
    this->isValid_ = false;
}

HttpRequest::HttpRequest(String request, String ipAddress) {
    this->request_ = request;
    this->set("REMOTE_ADDR", ipAddress);

    process();

    //Core::debugLn(this->toString());

    this->isValid_ = true;
}

HttpRequest::~HttpRequest() {
}

bool HttpRequest::isValid() {
    return this->isValid_;
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

            if (explodedSize > 0) {
                this->setHttpMethod(explodedRequest.at(0));

                if (explodedSize > 1) {
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

String HttpRequest::get(String key) {
    return this->data_[key];
}

String HttpRequest::getHttpMethod() {
    return this->get("HTTP_METHOD");
}

String HttpRequest::getUrl() {
    return this->get("REQUEST_URI");
}

StringMap HttpRequest::getQuery() {
    return this->query_;
}

void HttpRequest::set(String key, String value) {
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

void HttpRequest::setQueryParam(String key, String value) {
    this->query_[key] = value;
}

String HttpRequest::toString() {
    String output;

    for (auto pair : this->data_) {
        output.append(pair.first + ": " + pair.second + ENDL);
    }

    return output;
}

void HttpRequest::setQuery(String queryString) {
    this->set("QUERY_STRING", queryString);

    StringList data = queryString.explode("&");

    for (String row : data) {
        StringList keyVal = row.explode("=");

        if (keyVal.size() == 2) {
            this->setQueryParam(keyVal.at(0), keyVal.at(1));
        }
    }
}