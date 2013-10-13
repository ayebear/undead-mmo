// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "stringutils.h"
#include <fstream>

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
        for (auto& newLineChar: {cr, lf})
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

void StringUtils::cleanUp(std::string& str)
{
    stripNewLines(str); // Strip any CR or LF characters
    trimWhiteSpace(str); // Trim any whitespace characters (on the outsides)
    stripComments(str); // Strip any comments
}

int StringUtils::replaceAll(std::string& str, const std::string& findStr, const std::string& replaceStr)
{
    int count = 0;
    size_t pos = 0;
    // Keep searching for the string to find
    while ((pos = str.find(findStr, pos)) != std::string::npos)
    {
        // Replace the found string with the replace string
        str.replace(pos, findStr.length(), replaceStr);
        pos += replaceStr.size();
        ++count;
    }
    return count; // Return the number of occurrences that were replaced
}

void StringUtils::split(const std::string& inStr, const std::string& delim, std::vector<std::string>& outVec, bool allowEmpty)
{
    size_t start = 0;
    size_t end = 0;
    // Keep searching for the delimiters to split
    while ((end = inStr.find(delim, start)) != std::string::npos)
    {
        if (allowEmpty || start != end) // Always add the string if empty strings are allowed, otherwise make sure the string being added is not empty
            outVec.push_back(inStr.substr(start, end - start)); // Add the sub string between the delimiters to the vector
        start = end + delim.size();
    }
    // Get the last part of the string
    if (start < inStr.size())
        outVec.push_back(inStr.substr(start, inStr.size()));
}

void StringUtils::getLinesFromString(std::string inStr, std::vector<std::string>& lines)
{
    // First, search and replace all CRLF with LF, and then CR with LF
    while (replaceAll(inStr, "\r\n", "\n")); // This needs to be a while loop in case there is something like "\r\r\n"
    replaceAll(inStr, "\r", "\n");
    // Then, split the string on the LF characters into the vector
    split(inStr, "\n", lines, false);
}

bool StringUtils::readLinesFromFile(const std::string& filename, std::vector<std::string>& lines)
{
    bool status = false;
    std::ifstream file(filename, std::ifstream::in); // Open the file
    if (file.is_open())
    {
        std::string line;
        while (getline(file, line)) // Read a line
        {
            cleanUp(line); // Strip away unwanted characters
            if (!line.empty()) // If the line is not empty
                lines.push_back(line); // Store the line
        }
        status = true;
    }
    return status;
}
