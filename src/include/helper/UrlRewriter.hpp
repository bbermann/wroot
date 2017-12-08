#ifndef URLREWRITER_HPP
#define URLREWRITER_HPP

#include <regex>
#include "../type/String.hpp"

class UrlRewriter 
{
public:
    UrlRewriter()
    {
    }

    virtual ~UrlRewriter()
    {
    }

    String rewrite(String url)
    {
        std::regex rule("^.+$");
        std::smatch matches;

        if (!regex_match(url, matches, rule))
        {
            return url;
        }

        String rewrittenUrl("index.php/" + matches.str(0));
        return rewrittenUrl;
    }
};

#endif //URLREWRITER_HPP