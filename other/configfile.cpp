// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "configfile.h"
#include <fstream>
#include <iostream>

ConfigFile::ConfigFile()
{
}

ConfigFile::ConfigFile(const string& filename)
{
    loadConfigFile(filename);
}

ConfigFile::ConfigFile(const ConfigMap& defaultOptions)
{
    setDefaultOptions(defaultOptions);
}

ConfigFile::ConfigFile(const ConfigMap& defaultOptions, const string& filename)
{
    setDefaultOptions(defaultOptions);
    loadConfigFile(filename);
}

bool ConfigFile::loadConfigFile(const string& filename)
{
    configFilename = filename;
    vector<string> lines; // This is where the exact lines of the file will be stored into
    if (readLinesFromFile(configFilename, lines))
    {
        parseLines(lines);
        return true;
    }
    return false;
}

bool ConfigFile::writeConfigFile()
{
    return writeConfigFile(configFilename);
}

bool ConfigFile::writeConfigFile(const string& outputFilename)
{
    if (!outputFilename.empty())
    {
        string fileData = buildString();
        ofstream outFile(outputFilename, ofstream::out | ofstream::trunc);
        if (outFile.is_open())
        {
            outFile << fileData; // Write the string to the output file
            outFile.close();
            return true;
        }
    }
    return false;
}

Option& ConfigFile::getOption(const string& name)
{
    return options[name];
}

void ConfigFile::setDefaultOptions(const ConfigMap& defaultOptions)
{
    options.insert(defaultOptions.begin(), defaultOptions.end());
}

string ConfigFile::buildString()
{
    string configStr;
    for (auto& o: options)
    {
        if (o.second.getIsString()) // If it originally had quotes, make sure to include them
            configStr += o.first + " = \"" + o.second.asString() + "\"\n";
        else
            configStr += o.first + " = " + o.second.asString() + '\n';
    }
    return configStr;
}

bool ConfigFile::readLinesFromFile(const string& filename, vector<string>& lines)
{
    ifstream file(filename, ifstream::in); // Open the file
    if (file.is_open())
    {
        string line;
        while (getline(file, line)) // Read a line
        {
            stripNewLines(line); // Strip any CR or LF characters from the line
            stripComments(line); // Strip any comments from the line
            if (!line.empty()) // If the line is not empty
                lines.push_back(line); // Store the line
        }
        return true;
    }
    return false;
}

void ConfigFile::parseLines(vector<string>& lines)
{
    for (auto& line: lines) // Iterate through the vector of strings
    {
        size_t equalPos = line.find("="); // Find the position of the "=" symbol
        if (equalPos != string::npos && equalPos >= 1) // Ignore the line if there is no "=" symbol
        {
            string name, value;
            // Extract the name and value
            name = line.substr(0, equalPos);
            value = line.substr(equalPos + 1);
            // Trim any whitespace around the name and value
            trimWhiteSpace(name);
            trimWhiteSpace(value);
            // Remove outer quotes if any
            bool trimmedQuotes = trimQuotes(value);
            // Add/update the option in memory
            options[name] = value;
            if (trimmedQuotes)
                options[name].setIsString(true);
        }
    }
}

void ConfigFile::trimWhiteSpace(string& str)
{
    while (!str.empty() && isWhitespace(str.back())) // Loop until right side contains no whitespace
        str.pop_back(); // Remove last character
    while (!str.empty() && isWhitespace(str.front())) // Loop until left side contains no whitespace
        str.erase(str.begin()); // Remove first character
}

bool ConfigFile::trimQuotes(string& str)
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

void ConfigFile::stripNewLines(string& str)
{
    if (!str.empty())
    {
        for (auto& newLineChar: {'\r', '\n'})
        {
            auto pos = str.find(newLineChar);
            while (pos != string::npos)
            {
                str.erase(pos, 1);
                pos = str.find(newLineChar);
            }
        }
    }
}

void ConfigFile::stripComments(string& str)
{
    size_t minPos = string::npos;
    for (auto& cSymbol: {"//", "#", "::", ";"}) // Loop through the possible comment symbols
    {
        size_t found = str.find(cSymbol); // Look for that comment symbol
        // If the symbol was found and the found position is less than the current minimum position (or there is no current position)
        if (found != string::npos && (found < minPos || minPos == string::npos))
            minPos = found; // Set the new current minimum position to the one that was just found
    }
    if (minPos != string::npos) // If a comment symbol was found
        str.erase(minPos); // Remove everything after that
}

bool ConfigFile::isWhitespace(char c)
{
    // Just test if the char is a space or a tab
    return (c == ' ' || c == '\t');
}

bool ConfigFile::areQuotes(char c1, char c2)
{
    // Both characters must be the same
    // One of them must be either a single quote or a double quote
    // Only need to compare one char instead of both, because they must be equal due to the first check
    return ((c1 == c2) && (c1 == '"' || c1 == '\''));
}
