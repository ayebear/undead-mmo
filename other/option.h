// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef OPTION_H
#define OPTION_H

#include <string>
#include <memory>
#include "stringutils.h"
#include "range.h"

// This class can store a value of different types based on a string
class Option
{
    public:
        Option();
        Option(const std::string&);
        Option& operator=(const std::string&);

        // Sets all values to 0 and removes the range
        void reset();

        // Factory functions
        typedef Range::Rule Rule;
        template <class T> static Option create(T);
        template <class T> static Option create(T, Rule, T, Rule = Range::NoRule, T = 0);
        template <class T> static Option create(const std::string&, Rule, long, Rule = Range::NoRule, long = 0); // Possibly make this a non-template?

        // Setting will compute all possible types
        void setString(const std::string&);
        template <class T> void set(T);
        template <class T> void set(const std::string&);

        // Getting will simply return the precomputed values
        const std::string& asString() const;
        std::string asStringWithQuotes() const;
        int asInt() const;
        long asLong() const;
        float asFloat() const;
        double asDouble() const;
        bool asBool() const;

        // For determining if the option was originally read in as a string with quotes
        void setQuotes(bool);
        bool hasQuotes();

        // For setting the valid range
        template <class T> void setRange(Rule, T, Rule = Range::NoRule, T = 0, bool = false);
        void removeRange();

    private:
        std::string str; // Original data is always kept in here

        // When the data is interpreted, it is cached in these
        long number;
        double decimal;
        bool logical;

        bool quotes;

        // TODO: Use a unique_ptr or something, and when objects of this class are copied,
        // the range object will be created again and copied by value
        std::shared_ptr<Range> range;
};

template <class T>
Option Option::create(T data)
{
    Option tmp;
    tmp.set<T>(data);
    return tmp;
}

template <class T>
Option Option::create(T data, Rule ruleA, T numA, Rule ruleB, T numB)
{
    Option tmp;
    tmp.setRange<T>(ruleA, numA, ruleB, numB, false);
    tmp.set<T>(data);
    return tmp;
}

template <class T>
Option Option::create(const std::string& data, Rule ruleA, long numA, Rule ruleB, long numB)
{
    Option tmp;
    tmp.setRange<long>(ruleA, numA, ruleB, numB, true);
    tmp.set<const std::string&>(data);
    return tmp;
}

template <class T>
void Option::set(T data)
{
    if (!range || range->check<T>(data))
    {
        number = data;
        decimal = data;
        logical = (data != 0);
        str = StringUtils::toString<T>(data);
    }
}

template <class T>
void Option::set(const std::string& data)
{
    setString(data);
}

template <class T>
void Option::setRange(Rule ruleA, T numA, Rule ruleB, T numB, bool strLen)
{
    if (!range)
        range.reset(new Range());
    range->set<T>(ruleA, numA, ruleB, numB, strLen);
}

#endif
