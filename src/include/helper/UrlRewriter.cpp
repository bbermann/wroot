#include "UrlRewriter.hpp"
#include "../core/Core.hpp"

UrlRewriter::UrlRewriter()
{
}

UrlRewriter::~UrlRewriter()
{
}

String UrlRewriter::rewrite(String url)
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
            int iterator = 1;
            for (auto match : matches)
            {
                String dolarVar = match.str();
                String dolarIterator = "$" + std::to_string(iterator);
                rewrittenUrl = String::replace(rewrittenUrl, dolarIterator, dolarVar);

                ++iterator;
            }
            return rewrittenUrl;
        }
    }

    return url;
}

String UrlRewriter::getScriptPath(String url)
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
            int iterator = 1;
            for (auto match : matches)
            {
                String dolarIterator = "$" + std::to_string(iterator);
                rewrittenUrl = String::replace(rewrittenUrl, dolarIterator, "");

                ++iterator;
            }
            return rewrittenUrl;
        }
    }

    return url.trim();
}