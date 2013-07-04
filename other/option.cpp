// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include <iostream>
#include "option.h"

Option::Option():
    str(""),
    number(0),
    decimal(0),
    logical(false),
    isString(false)
{}

Option::Option(const std::string& data) { setString(data); }

Option& Option::operator=(const std::string& data)
{
    setString(data);
    return *this;
}

void Option::setString(const std::string& data)
{
    isString = false;
    str = data;
    std::istringstream tmp(data);
    if (tmp >> decimal) // Try reading a double from it
    {
        // Successfully parsed number!
        number = decimal; // Truncate to an int
        logical = (decimal != 0); // Deduce the number as a boolean
    }
    else
    {
        // Could not parse number, reverting to default values
        number = 0;
        decimal = 0;
        logical = strToBool(data); // Try to convert to boolean ("True", "False", or something similar)
    }
}

void Option::setInt(const int& data)
{
    number = data;
    decimal = data;
    logical = (data != 0);
    str = toString<int>(data);
}

void Option::setDouble(const double& data)
{
    number = data;
    decimal = data;
    logical = (data != 0);
    str = toString<double>(data);
}

void Option::setBool(const bool& data)
{
    number = data;
    decimal = data;
    logical = data;
    str = logical ? "true" : "false";
}

const std::string& Option::asString() const { return str; }

const int& Option::asInt() const { return number; }

const double& Option::asDouble() const { return decimal; }

const bool& Option::asBool() const { return logical; }

void Option::setIsString(bool setting) { isString = setting; }

bool Option::getIsString() { return isString; }

bool strToBool(const std::string& data)
{
    std::string tmp(data); // Copy the string to a temporary one
    for (auto& c: tmp)
        c = tolower(c); // Make all of the characters lowercase
    std::size_t found = tmp.find("true"); // If "true" exists somewhere then the boolean is true
    return (found != std::string::npos);
}
