// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef OPTION_H
#define OPTION_H

#include <string>
#include <sstream>

bool strToBool(const std::string&);

// Converts most types to strings using a string stream
template <class T>
std::string toString(T data)
{
    std::ostringstream tmp;
    tmp << data;
    return tmp.str();
}

// This class can store a value of different types based on a string
class Option
{
    public:
        Option();
        Option(const std::string&);
        Option& operator=(const std::string&);

        // Setting will compute all possible types
        void setString(const std::string&);
        void setInt(const int&);
        void setDouble(const double&);
        void setBool(const bool&);

        // Getting will simply return the precomputed values
        const std::string& asString() const;
        const int& asInt() const;
        const double& asDouble() const;
        const bool& asBool() const;

        // For determining if the option was originally read in as a literal string with quotes
        void setIsString(bool);
        bool getIsString();

    private:
        std::string str; // Original data is always kept in here
        // When the data is interpreted, it is cached in these
        int number;
        double decimal;
        bool logical;

        bool isString;
};

#endif
