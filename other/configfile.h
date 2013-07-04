// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <map>
#include <vector>
#include <string>
#include "option.h"

using namespace std;

/*
TODO:
    Ignore comment symbols in strings!!!
    Figure out a good way to support valid ranges or types for settings.
        This could be solved with some sort of data structure in memory that could be used by the ConfigFile class,
            passed in by something else, which the settings would be loaded and "fitted" into.
        With this, settings will never be invalid or out of range!
    Support multi-line comments.
    Config file writing should somehow preserve formatting and comments to an extent.
        Maybe it could "merge" the contents into the file, new values in existing options would be updated,
            new options would be appended to the end of the file.
        Right now it is completely destructive, it doesn't preserve ordering, comments, invalid data, or whitespace.
    Handle escaping special characters such as new lines: str = "\n"
        This might not be needed.
*/

/*
This class reads simple generic config files in a format like this:
someVariable = someValue

The first part is the name of the setting, the second is its value.
There must be an equals symbol in between!

These can be different types too, and named almost anything.
someNumber = 500
someDecimal = 3.1415926535 // Decimals are stored as double precision floats
someBoolean = True
anotherBool = false
bool3 = FALSE // Booleans are not case sensitive
bool4 = 0 // Zero is false
bool5 = 100 // A non-zero value is true

How strings are handled:
someString =    this is a test
This string is interpreted as "this is a test". It ignores all whitespace "around" the data.
It is recommended to use quotes for strings, so that it is interpreted as you expect:
someString = "    this is a test"
Symbols in strings work fine too, including quotes:
str = "!@#$%^&*()"""""""_+-="
The first and last quote are used for determining what is contained in the string.

Whitespace is ignored around setting names.
Semicolons CANNOT be used to separate lines, only new lines can.
Comments can be made with most symbols (//, #, ::, and ;)
Comments can be on lines with settings, not just on their own lines.
*/

class ConfigFile
{
    public:
        typedef map<string,Option> ConfigMap;

        ConfigFile(); // Would create an empty config file object
        ConfigFile(const string&); // Loads options from a file
        ConfigFile(const ConfigMap&); // Loads default options from memory
        ConfigFile(const ConfigMap&, const string&); // Loads default options and options from a file

        bool loadConfigFile(const string&); // Loads options from a file
        bool writeConfigFile(); // Saves current options to the file last loaded
        bool writeConfigFile(const string&); // Saves current options to a file

        Option& getOption(const string&); // Returns a reference to an option with the specified name. If it does not exist, it will be automatically created
        void setDefaultOptions(const ConfigMap&); // Just sets initial values in the map from another map in memory
        string buildString(); // This just builds up a string of all of the options in memory

    private:
        bool readLinesFromFile(const string&, vector<string>&); // Simply reads a file into memory in separate lines
        void parseLines(vector<string>&); // Processes the lines in memory and adds them to the options map
        void trimWhiteSpace(string&); // Trims all whitespace on both sides of the string
        bool trimQuotes(string&); // Trims quotes on ends of string, returns true if the string was modified
        void stripComments(string&); // Removes all comments from a string
        bool isWhitespace(char); // Returns true if the character is a form of whitespace
        bool areQuotes(char, char); // Returns true if both characters are either single or double quotes

        string configFilename;
        ConfigMap options;
};

#endif
