#include "String.hpp"

using namespace std;

String::String(string val)
: string(val)
{
}

String::String(const char* cval)
: string(cval)
{
}

String::~String()
{
}

/*
bool String::contains(const char *str)
{
	string str_val = str;

	return this->contains(str_val);
}

bool String::endsWith(const char *str)
{
	string str_val = str;

	return this->endsWith(str_val);
}
*/

bool String::contains(string str)
{
    string cmp_value = str;

    for (unsigned it_this = 0; it_this < this->length(); it_this++)
    {
        string test_string = this->substr(it_this, str.length());

        if (test_string == cmp_value)
        {
            return true;
        }
    }

    return false;
}

bool String::endsWith(string str)
{
	if (str.length() > this->length())
	{
		return false;
	}

	string this_end = this->substr(this->length() - str.length(), this->length());
	string str_end = str;

	return (this_end == str_end);
}

StringList String::explode(String separator)
{
    StringList string_list;
    String::explode(*this, separator, &string_list);
    return string_list;
}

void String::explode(string str, string separator, StringList* results)
{
    string work_copy = str;
    size_t found;
    found = work_copy.find_first_of(separator);

    while (found != string::npos)
    {
        if (found > 0)
        {
            results->push_back(work_copy.substr(0, found));
        }

        work_copy = work_copy.substr(found + 1);
        found = work_copy.find_first_of(separator);
    }

    if (work_copy.length() > 0)
    {
        results->push_back(work_copy);
    }
}

string String::replace(string& str, string from, string to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

string String::toLower(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);

    return str;
}

string String::toUpper(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::toupper);

    return str;
}

string String::trim(string str)
{
    str.erase(str.begin(), find_if(str.begin(), str.end(),
                                   [](char& ch)->bool
                                   {
                                       return !isspace(ch); }));
    str.erase(find_if(str.rbegin(), str.rend(),
                      [](char& ch)->bool
                      {
                          return !isspace(ch); }).base(), str.end());

    return str;
}

StringList String::regex_search(string text, string regexp)
{
    std::smatch match;
    std::regex exp(regexp);
    StringList strret;

    std::regex_search(text, match, exp);
    for (size_t i = 0; i < match.size(); ++i)
    {
        std::ssub_match sub_match = match[i];
        std::string sub_match_str = sub_match.str();
        strret.insert(strret.end(), sub_match.str());
    }

    return strret;
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
    }).base(), this->end());
    return *this;
}

// trim from both ends (in place)
String String::trim() {
    ltrim();
    rtrim();
    return *this;
}