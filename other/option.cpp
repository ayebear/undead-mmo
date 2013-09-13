// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "option.h"

Option::Option():
    str(""),
    number(0),
    decimal(0),
    logical(false),
    quotes(false)
{
}

Option::Option(const std::string& data)
{
    set<int>(0);
    setString(data);
}

Option& Option::operator=(const std::string& data)
{
    setString(data);
    return *this;
}

void Option::reset()
{
    removeRange();
    set<int>(0);
}

void Option::setString(const std::string& data)
{
    if (!range || range->check<const std::string&>(data)) // Check if the string's length is in range
    {
        std::istringstream tmpStream(data);
        double tmpDec = 0;
        bool parsedNumber = (tmpStream >> tmpDec); // Try reading a number from the string
        if (!range || range->check<double>(tmpDec)) // Check if the number is in range, if there is a range at all
        {
            decimal = tmpDec; // Set the decimal from the temporary one
            number = decimal; // Truncate to an int
            quotes = !parsedNumber; // No quotes around a number
            logical = (parsedNumber ? (decimal != 0) : StringUtils::strToBool(data)); // Convert to a boolean
            str = data; // Set the string
        }
    }
}

const std::string& Option::asString() const { return str; }

std::string Option::asStringWithQuotes() const
{
    if (quotes)
        return '"' + str + '"';
    else
        return str;
}

int Option::asInt() const { return number; }

long Option::asLong() const { return number; }

float Option::asFloat() const { return decimal; }

double Option::asDouble() const { return decimal; }

bool Option::asBool() const { return logical; }

void Option::setQuotes(bool setting) { quotes = setting; }

bool Option::hasQuotes() { return quotes; }

void Option::removeRange() { range.reset(); }
