#ifndef URLREWRITERULE_HPP
#define URLREWRITERULE_HPP

#include <regex>
#include "../type/String.hpp"

class UrlRewriteRule {
public:
    UrlRewriteRule() {}

    ~UrlRewriteRule() {}

    std::regex input;
    String output;
};

#endif //URLREWRITERULE_HPP
