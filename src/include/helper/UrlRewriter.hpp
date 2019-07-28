#ifndef URLREWRITER_HPP
#define URLREWRITER_HPP

#include <regex>
#include "../type/String.hpp"
#include "../type/UrlRewriteRule.hpp"

class UrlRewriter {
public:
    UrlRewriter();

    virtual ~UrlRewriter();

    String rewrite(String url);

    String getScriptPath(String url);
};

#endif //URLREWRITER_HPP