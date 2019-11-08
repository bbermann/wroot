#ifndef STRING_HPP
#define STRING_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <memory>
#include <map>

class String;

typedef std::vector<std::string> StringList;
typedef std::map<std::string, std::string> StringMap;

class String : public std::string {
public:
    String(const char *val = "") noexcept;

    String(const std::string &val) noexcept;

    ~String();

    bool contains(const String &subStr) const;

    static bool contains(const String &str, const String &subStr);

    bool endsWith(const String &end) const;

    static bool endsWith(const String &str, const String &end);

    bool startsWith(const String &str) const;

    static bool startsWith(const String &str, const String &begin);

    StringList regex_search(const String &regexp);

    StringList explode(const String &separator) const;

    static StringList explode(const String &str, const String &separator);

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
