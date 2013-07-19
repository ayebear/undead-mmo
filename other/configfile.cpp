// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "configfile.h"
#include <fstream>
#include <iostream>
#include "stringutils.h"

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

bool ConfigFile::optionExists(const string& name)
{
    return (options.find(name) != options.end());
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
            StringUtils::stripNewLines(line); // Strip any CR or LF characters from the line
            StringUtils::stripComments(line); // Strip any comments from the line
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
            StringUtils::trimWhiteSpace(name);
            StringUtils::trimWhiteSpace(value);
            // Remove outer quotes if any
            bool trimmedQuotes = StringUtils::trimQuotes(value);
            // Add/update the option in memory
            options[name] = value;
            if (trimmedQuotes)
                options[name].setIsString(true);
        }
    }
}
