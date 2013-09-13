#ifndef RANGE_H
#define RANGE_H

#include <string>

// This class stores an optional valid range, so that Option objects can determine if their value is in range
class Range
{
    public:
        enum Type
        {
            NoType = 0, // No range applied, so anything is allowed
            Numeric, // Size of a number
            String // Length of string
        };

        enum Rule
        {
            NoRule = 0,
            LT, // Less than (<)
            LTE, // Less than or equal to (<=)
            GT, // Greater than (>)
            GTE // Greater than or equal to (>=)
        };

        Range();

        template <class T> void set(Rule, T, Rule = NoRule, T = 0, bool = false);

        // These return true if the value is in the valid range or the type is not correct.
        template <class T> bool check(T);
        template <class T> bool check(const std::string&);

        // These will use the specified default value if the value is out of range
        template <class T> bool fix(T&, T);
        template <class T> bool fix(std::string&, const std::string&);

        // These will truncate or try to fit the values into their range
        //template <class T> bool fix(T&);
        //template <class T> bool fix(std::string&);

        bool hasRange() const; // Returns true if type != NoType
        void reset(); // Resets rules and type back to NoType
        Type getType(); // Returns the type of range which was originally set

    private:
        template <class T> bool ruleCheck(Rule, T, T);

        double dec1;
        double dec2;
        Rule rule1;
        Rule rule2;
        Type type;
};

template <class T>
void Range::set(Rule ruleA, T numA, Rule ruleB, T numB, bool strLen)
{
    dec1 = numA;
    dec2 = numB;
    rule1 = ruleA;
    rule2 = ruleB;
    type = (strLen ? String : Numeric);
}

template <class T>
bool Range::check(T num)
{
    return (type != Numeric ||
    (ruleCheck<double>(rule1, dec1, (double)num) &&
    ruleCheck<double>(rule2, dec2, (double)num)));
}

template <class T>
bool Range::check(const std::string& str)
{
    return (type != String ||
    (ruleCheck<long>(rule1, (long)dec1, (long)str.size()) &&
    ruleCheck<long>(rule2, (long)dec2, (long)str.size())));
}

template <class T>
bool Range::fix(T& num, T defaultNum)
{
    if (!check(num))
    {
        num = defaultNum;
        return true;
    }
    return false;
}

template <class T>
bool Range::fix(std::string& str, const std::string& defaultStr)
{
    if (!check(str))
    {
        str = defaultStr;
        return true;
    }
    return false;
}

template <class T>
bool Range::ruleCheck(Rule rule, T ruleNum, T num)
{
    /*
    Returns true when:
        There is no rule set OR
        The value is in range
    Returns false when:
        The value is out of range
    */
    bool status = true;
    if (rule != NoRule)
    {
        status = ((rule == LT && num < ruleNum) ||
                (rule == LTE && num <= ruleNum) ||
                (rule == GT && num > ruleNum) ||
                (rule == GTE && num >= ruleNum));

        //const std::string ruleStrs[] = {" ??? ", " < ", " <= ", " > ", " >= "};
        //std::cout << "ruleCheck(): status = " << status << ", " << num << ruleStrs[rule] << ruleNum << "\n";
    }
    return status;
}

#endif
