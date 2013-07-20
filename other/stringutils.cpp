// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "stringutils.h"

void StringUtils::trimWhiteSpace(std::string& str)
{
    while (!str.empty() && isWhitespace(str.back())) // Loop until right side contains no whitespace
        str.pop_back(); // Remove last character
    while (!str.empty() && isWhitespace(str.front())) // Loop until left side contains no whitespace
        str.erase(str.begin()); // Remove first character
}

bool StringUtils::trimQuotes(std::string& str)
{
    if (str.size() >= 2) // If there are at least 2 characters in the string
    {
        if (areQuotes(str.front(), str.back())) // If the outsides of the string have quotes
        {
            // Remove the quotes
            str.pop_back();
            str.erase(str.begin());
            return true;
        }
    }
    return false;
}

void StringUtils::stripNewLines(std::string& str)
{
    if (!str.empty())
    {
        for (auto& newLineChar: {'\r', '\n'}) // Remove CR and LF characters
        {
            auto pos = str.find(newLineChar); // Find the character
            while (pos != std::string::npos) // Continue removing the characters until there are none left
            {
                str.erase(pos, 1); // Remove the character
                pos = str.find(newLineChar); // Find another character to remove
            }
        }
    }
}

void StringUtils::stripComments(std::string& str)
{
    size_t minPos = std::string::npos;
    for (auto& cSymbol: {"//", "#", "::", ";"}) // Loop through the possible comment symbols
    {
        size_t found = str.find(cSymbol); // Look for that comment symbol
        // If the symbol was found and the found position is less than the current minimum position (or there is no current position)
        if (found != std::string::npos && (found < minPos || minPos == std::string::npos))
            minPos = found; // Set the new current minimum position to the one that was just found
    }
    if (minPos != std::string::npos) // If a comment symbol was found
        str.erase(minPos); // Remove everything after that
}

bool StringUtils::isWhitespace(char c)
{
    // Just test if the char is a space or a tab
    return (c == ' ' || c == '\t');
}

bool StringUtils::areQuotes(char c1, char c2)
{
    // Both characters must be the same
    // One of them must be either a single quote or a double quote
    // Only need to compare one char instead of both, because they must be equal due to the first check
    return ((c1 == c2) && (c1 == '"' || c1 == '\''));
}
