// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "configfile.h"
#include <fstream>
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

ConfigFile::ConfigFile(const Section& defaultOptions, const string& section)
{
    setDefaultOptions(defaultOptions, section);
}

ConfigFile::ConfigFile(const string& filename, const ConfigMap& defaultOptions)
{
    setDefaultOptions(defaultOptions);
    loadConfigFile(filename);
}

ConfigFile::ConfigFile(const string& filename, const Section& defaultOptions, const string& section)
{
    setDefaultOptions(defaultOptions, section);
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

Option& ConfigFile::getOption(const string& name, const string& section)
{
    return options[getCurrentSection(section)][name];
}

Option& ConfigFile::operator[](const string& name)
{
    return options[currentSection][name];
}

bool ConfigFile::optionExists(const string& name, const string& section)
{
    auto sectionFound = options.find(getCurrentSection(section));
    return (sectionFound != options.end() && sectionFound->second.find(name) != sectionFound->second.end());
}

void ConfigFile::setDefaultOptions(const ConfigMap& defaultOptions)
{
    options.insert(defaultOptions.begin(), defaultOptions.end());
}

void ConfigFile::setDefaultOptions(const Section& defaultOptions, const string& section)
{
    options[getCurrentSection(section)].insert(defaultOptions.begin(), defaultOptions.end());
}

string ConfigFile::buildString()
{
    string configStr;
    for (auto& section: options) // Go through all of the sections
    {
        if (!section.first.empty())
            configStr += '[' + section.first + "]\n"; // Add the section line if it is not blank
        for (auto& o: section.second) // Go through all of the options in this section
            configStr += o.first + " = " + o.second.asStringWithQuotes() + '\n'; // Include the original quotes if any
        configStr += '\n';
    }
    if (!configStr.empty() && configStr.back() == '\n')
        configStr.pop_back(); // Strip the extra new line at the end
    return configStr;
}

void ConfigFile::setSection(const string& section)
{
    currentSection = section;
}

bool ConfigFile::eraseOption(const string& name, const string& section)
{
    auto sectionFound = options.find(getCurrentSection(section));
    if (sectionFound != options.end()) // If the section exists
        return (sectionFound->second.erase(name) > 0); // Erase the option
    return false;
}

bool ConfigFile::eraseSection(const string& section)
{
    return (options.erase(getCurrentSection(section)) > 0); // Erase the section
}

void ConfigFile::clear()
{
    options.clear(); // Clear all of the sections and options
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
            StringUtils::trimWhiteSpace(line); // Trim any whitespace characters (on the outsides)
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
    string section = "";
    for (auto& line: lines) // Iterate through the vector of strings
    {
        if (isSection(line))
            parseSectionLine(line, section); // Example: "[Section]"
        else
            parseOptionLine(line, section); // Example: "Option = Value"
    }
}

bool ConfigFile::isSection(const string& str)
{
    return (str.size() >= 2 && str.front() == '[' && str.back() == ']');
}

void ConfigFile::parseSectionLine(const string& line, string& section)
{
    section = line.substr(1, line.size() - 2); // Set the current section
    options[section]; // Add that section to the map
}

void ConfigFile::parseOptionLine(const string& line, const string& section)
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
        options[section][name] = value;
        if (trimmedQuotes) // If quotes were removed
            options[section][name].setQuotes(true); // Add quotes to the option
    }
}

const string& ConfigFile::getCurrentSection(const string& section)
{
    return (section.empty() ? currentSection : section);
}
