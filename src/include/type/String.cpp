#include "String.hpp"

using namespace std;

String::String(const char *val) noexcept : string(val) {
}

String::String(const string &val) noexcept : string(val) {
}

String::~String() = default;

bool String::contains(const String &subStr) const {
    return String::contains(*this, subStr);
}

bool String::contains(const String &str, const String &subString) {
    for (size_t it_this = 0; it_this < str.length(); it_this++) {
        string test_string = str.substr(it_this, subString.length());

        if (test_string == subString) {
            return true;
        }
    }

    return false;
}

bool String::endsWith(const String &end) const {
    return String::endsWith(*this, end);
}

bool String::endsWith(const String &str, const String &end) {
    if (end.length() > str.length()) {
        return false;
    }

    string this_end = str.substr(str.length() - end.length(), str.length());
    return (this_end == end);
}

bool String::startsWith(const String &begin) const {
    return String::startsWith(*this, begin);
}

bool String::startsWith(const String &str, const String &begin) {
    if (begin.length() > str.length()) {
        return false;
    }

    string this_begin = str.substr(0, begin.length());
    return (this_begin == begin);
}

StringList String::explode(const String &separator) const {
    return String::explode(*this, separator);
}

StringList String::explode(const String &str, const String &separator) {
    StringList results;
    string work_copy = str;
    size_t found;
    found = work_copy.find_first_of(separator);

    while (found != string::npos) {
        if (found > 0) {
            results.push_back(work_copy.substr(0, found));
        }

        work_copy = work_copy.substr(found + 1);
        found = work_copy.find_first_of(separator);
    }

    if (!work_copy.empty()) {
        results.push_back(work_copy);
    }

    return results;
}

String &String::replace(String &str, const String &from, const String &to) {
    size_t start_pos = 0;

    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        ((std::string &) str).replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }

    return str;
}

String &String::replace_last(String &str, const String &from, const String &to) {
    auto found = str.rfind(from);

    if (found != std::string::npos) {
        ((std::string &) str).replace(found, from.length(), to);
    }

    return str;
}

String &String::toLower(String &str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);

    return str;
}

String &String::toUpper(String &str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);

    return str;
}

String &String::trim(String &str) {
    str.erase(str.begin(), find_if(str.begin(), str.end(),
                                   [](char &ch) -> bool { return !isspace(ch); }));
    str.erase(find_if(str.rbegin(), str.rend(),
                      [](char &ch) -> bool { return !isspace(ch); })
                      .base(),
              str.end());

    return str;
}

StringList String::regex_search(const String &regexp) {
    return String::regex_search(*this, regexp);
}

StringList String::regex_search(const String &text, const String &regexp) {
    std::smatch matches;
    std::regex exp(regexp);
    StringList stringList;

    std::regex_search(text, matches, exp);
    for (size_t i = 0; i < matches.size(); ++i) {
        std::ssub_match sub_match = matches[i];
        std::string sub_match_str = sub_match.str();
        stringList.insert(stringList.end(), sub_match.str());
    }

    return stringList;
}

// trim from start (in place)
String String::ltrim() {
    this->erase(this->begin(), std::find_if(this->begin(), this->end(), [](int ch) {
        return !std::isspace(ch);
    }));
    return *this;
}

// trim from end (in place)
String String::rtrim() {
    this->erase(std::find_if(this->rbegin(), this->rend(), [](int ch) {
                    return !std::isspace(ch);
                })
                        .base(),
                this->end());
    return *this;
}

// trim from both ends (in place)
String String::trim() {
    ltrim();
    rtrim();
    return *this;
}