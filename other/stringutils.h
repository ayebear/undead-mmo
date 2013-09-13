// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <sstream>

namespace StringUtils
{
    void trimWhiteSpace(std::string&); // Trims all whitespace on both sides of the string
    bool trimQuotes(std::string&); // Trims quotes on ends of string, returns true if the string was modified
    void stripNewLines(std::string&); // Removes all new lines/carriage returns from a string
    void stripComments(std::string&); // Removes all comments from a string
    bool isWhitespace(char); // Returns true if the character is a form of whitespace
    bool areQuotes(char, char); // Returns true if both characters are either single or double quotes
    bool strToBool(std::string); // Parses a string to determine its boolean value
    template <class T> std::string toString(T data); // Converts most types to strings using a string stream
    template <class T> std::string toString(bool data); // Converts a bool to a string
}

template <class T>
std::string StringUtils::toString(T data)
{
    std::ostringstream tmp;
    tmp.precision(16);
    tmp << data;
    return tmp.str();
}

template <class T>
std::string StringUtils::toString(bool data)
{
    return (data ? "true" : "false");
}

#endif
