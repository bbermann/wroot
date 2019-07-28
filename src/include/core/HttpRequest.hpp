#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "../type/String.hpp"

class HttpRequest {
public:
    HttpRequest();

    HttpRequest(String request, String ipAddress);

    virtual ~HttpRequest();

    String get(String key);

    String getHttpMethod();

    String getUrl();

    StringMap getQuery();

    String toString();

    bool isValid();

protected:
    void set(String key, String value);

    void setHttpMethod(String method);

    void setUrl(String url);

    void setQueryParam(String key, String value);

    void setQuery(String queryString);

    void process();

private:
    String request_;
    StringMap data_;
    StringMap query_;
    bool isValid_ = true;
};

#endif //HTTPREQUEST_HPP