// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>

namespace StringUtils
{
    void trimWhiteSpace(std::string&); // Trims all whitespace on both sides of the string
    bool trimQuotes(std::string&); // Trims quotes on ends of string, returns true if the string was modified
    void stripNewLines(std::string&); // Removes all new lines/carriage returns from a string
    void stripComments(std::string&); // Removes all comments from a string
    bool isWhitespace(char); // Returns true if the character is a form of whitespace
    bool areQuotes(char, char); // Returns true if both characters are either single or double quotes
}

#endif
