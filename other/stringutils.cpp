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
    size_t rightQuote = str.rfind('"'); // Try to find the rightmost quote
    size_t startPos = (rightQuote == std::string::npos) ? 0 : rightQuote; // Start at the right quote position if it was found, otherwise start at the beginning
    for (const std::string& commentSymbol: {"//", "#", "::", ";"}) // Loop through the possible comment symbols
    {
        if (str.compare(0, commentSymbol.size(), commentSymbol) == 0) // Check the very beginning of the string for the comment symbol
        {
            minPos = 0; // Beginning of the string
            break; // No need to look for any more symbols
        }
        size_t found = str.find(commentSymbol, startPos); // Look for that comment symbol (or after the rightmost quote if it was found)
        // If the symbol was found, and the found position is less than the current minimum position (or there is no current position),
        // and if the rightmost quote exists, the comment symbol must be more to the right than it (comment symbols should be allowed in strings)
        if (found != std::string::npos && (minPos == std::string::npos || found < minPos) && (rightQuote == std::string::npos || found > rightQuote))
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
    // Both of them must be either single quotes or double quotes
    // Only need to compare one char instead of both, because they must be equal due to the first check
    return ((c1 == c2) && (c1 == '"' || c1 == '\''));
}

bool StringUtils::strToBool(std::string data)
{
    for (char& c: data)
        c = tolower(c); // Make all of the characters lowercase
    std::size_t found = data.find("true"); // If "true" exists somewhere then the boolean is true
    return (found != std::string::npos);
}
