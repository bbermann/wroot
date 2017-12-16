#ifndef URLREWRITER_HPP
#define URLREWRITER_HPP

#include <regex>
#include "../type/String.hpp"
#include "../type/UrlRewriteRule.hpp"

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
        Core::ThreadMutex.lock();
        std::vector<UrlRewriteRule> rewriteRules = Core::UrlRewriteRules;
        Core::ThreadMutex.unlock();

        for (auto rule : rewriteRules)
        {
            String rewrittenUrl = rule.output;
            std::smatch matches;

            if (regex_match(url, matches, rule.input))
            {
                int iterator = 0;
                for (auto match : matches)
                {
                    ++iterator;
                    String dolarVar = match.str();
                    String dolarIterator = "$" + std::to_string(iterator);
                    rewrittenUrl = String::replace(rewrittenUrl, dolarIterator, dolarVar);
                }
                return rewrittenUrl;
            }
        }

        return url;
    }
};

#endif //URLREWRITER_HPP