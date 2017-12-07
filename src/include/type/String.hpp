#ifndef STRING_HPP
#define STRING_HPP

#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <regex>
#include <memory>

typedef std::vector<std::string> StringList;

class String : public std::string {
public:
    String(const char* val = "");
    String(std::string val);
    ~String();
    
    /*inline virtual String operator=(const char* str) { 
        return static_cast<std::string>(str);
    }*/

    /*bool contains(const char *str);
    bool endsWith(const char *str);*/

	bool contains(std::string str); 
	bool endsWith(std::string str);

    StringList explode(String separator);
    
    static void explode(std::string str, std::string separator, StringList* results);
    static std::string replace(std::string& str, std::string from, std::string to);
    static std::string toLower(std::string str);
    static std::string toUpper(std::string str);
    static std::string trim(std::string str);
    static StringList regex_search(std::string text, std::string regexp);
    
    // trim from start (in place)
    String ltrim();

    // trim from end (in place)
    String rtrim();

    // trim from both ends (in place)
    String trim();
};

#endif // STRING_HPP
