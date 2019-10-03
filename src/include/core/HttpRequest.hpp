#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include "../type/String.hpp"

class HttpRequest {
public:
    HttpRequest(const String &request, const String &ipAddress);

    virtual ~HttpRequest();

    String get(const String &key) const;

    String getHttpMethod() const;

    String getUrl() const;

    StringMap getQuery() const;

    String toString() const;

protected:
    void set(const String &key, const String &value);

    void setHttpMethod(String method);

    void setUrl(String url);

    void setQueryParam(const String &key, const String &value);

    void setQuery(const String &queryString);

    void process();

private:
    String request_;
    StringMap data_;
    StringMap query_;
};

#endif //HTTPREQUEST_HPP