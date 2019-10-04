#ifndef STRING_HPP
#define STRING_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <memory>
#include <map>

typedef std::vector <std::string> StringList;
typedef std::map <std::string, std::string> StringMap;

class String : public std::string {
public:
    String(const char *val = "");

    String(const std::string &val);

    ~String();

    bool contains(const String &str) const;

    bool endsWith(const String &str) const;

    bool startsWith(const String &str) const;

    StringList regex_search(const String &regexp);

    StringList explode(const String &separator) const;

    static void explode(const String &str, const String &separator, StringList *results);

    static String& replace(String &str, const String &from, const String &to);

    static String& replace_last(String &str, const String &from, const String &to);

    static String& toLower(String &str);

    static String& toUpper(String &str);

    static String& trim(String &str);

    static StringList regex_search(const String &text, const String &regexp);

    // trim from start (in place)
    String ltrim();

    // trim from end (in place)
    String rtrim();

    // trim from both ends (in place)
    String trim();
};

#endif // STRING_HPP
