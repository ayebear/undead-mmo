/*Copyright (c) <2014> Kevin Miller - KevM1227@gmail.com

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/

#ifndef _SERIALIZER_H
#define _SERIALIZER_H

#include <set>
#include <sstream>
#include <string>

/**\brief Serializer -
*           Converts between a string and a value using a specified format string.
*           Users specifiy a format to indicate how a string should be converted to
*           a list of values. Inspired by printf and scanf.
*
*           e.g. serializer.serialize("%, % %", 5, 10, "Hello"); would produce the following output: "5, 10, Hello"
*
*           int x, y, z;
*           std::string aa;
*           serializer.deSerialize("% %, %s %", "540 30, 'A string' 10", x, y, aa, z");
*
*           x : 540
*           y : 30
*           aa : A string
*           z : 10
*
*           The escape character, conversion character, and the outer string markers may be modified.
*           To add more escapable characters, simply add them to the set's initializer list in the implementation file.
*
*@author Kevin Miller
*@version 2-14-14
*/
class Serializer
{
    public:

        static const char conversionChar = '%';
        static const char escapeChar = '/';
        static const char stringMarker = '\'';


        template<typename T, typename ... Args>
        std::string serialize(const std::string& formatStr, const T& first, Args&&... others);
        std::string serialize(const std::string&) { return (""); }

        template<typename T, typename ... Args>
        bool deSerialize(std::string formatStr, std::string valuesStr, T& first, Args&&... others);
        bool deSerialize(std::string, const std::string&) { return true;}

    private:

        std::string getNextValue(std::string&, char);
        bool moveToNextConversionChar(std::string&, std::string&);

        template<typename T>
        void convertStrToValue(const std::string& strValue, T& value);

        void removeAllEscapeChars(std::string&);
        void addEscapeCharacters(std::string&);

        //If there are any characters that can be escaped, put them in the initializer list in the implementation file.
        static const std::set<std::string> escapableCharacters;

};


std::stringstream& operator>>(std::stringstream&, std::string&);
std::stringstream& operator<<(std::stringstream&, std::string&);


/** \brief Convert a list of values to a string in the given format
 *
 * \param formatStr The way the values string should be formatted.
 *                  '%' or some other character is a used to indicate where a value should be inserted.
 *
 * \param valuesStr The string containing the values to be converted.
 *
 * \param first The first value to insert into the string.
 * \param others Any other values to be inserted.
 * \return A string in the given format containing the converted values.
 *
 */
template<typename T, typename ... Args>
std::string Serializer::serialize(const std::string& formatStr, const T& first, Args&&... others)
{
    bool found = false;
    std::string str("");
    std::stringstream ss;
    unsigned int index = 0;

    auto it = formatStr.begin();
    while(it != formatStr.end() && !found)
    {
        if(*it == conversionChar)
        {
            std::string temp("");
            ss << first;
            ss >> temp;
            addEscapeCharacters(temp);
            if( *( it + 1 ) == 's')
            {
                temp = stringMarker + temp + stringMarker;
                ++index;
            }

            str += temp;
            found = true;
        }
        else
            str += *it;

        ++it;
        ++index;
    }
    if(it != formatStr.end() && sizeof ... (others) > 0)
        return str + serialize(formatStr.substr(index), others...);

    return str;
}

/** \brief Convert a string with values in it, formatted the way specified in the format string, to each variable given.
 *
 * \param formatStr The way the values string should be formatted. % is a placeholder for a value. Use %s if the value
 *                  has is a string with spaces in it.
 *
 * \param valuesStr The string containing the values to be converted.
 *
 * \param first The location to store the first value found.
 * \param others The locations to store the rest of the values.
 * \return True the if values string could be parsed. False if not.
 *
 */
template<typename T, typename ... Args>
bool Serializer::deSerialize(std::string formatStr, std::string valuesStr, T& first, Args&&... others)
{
    //Look for the next conversion character while iterating through both strings.
    if(!moveToNextConversionChar(formatStr, valuesStr))
        return false;

    std::string value("");

    //If the value is a string
    if(formatStr[1] == 's')
    {
        //Check if the first character in the value is a string marker
        if(valuesStr[0] == stringMarker)
        {
            //Remove the conversion character from the format sting and the opening string marker from the value.
            valuesStr = valuesStr.substr(1);
            formatStr = formatStr.substr(1);
            value = getNextValue(valuesStr, stringMarker);
        }
    }
    //Otherwise get the value and use the character after the conversion character as the end of the value
    else
        value = getNextValue(valuesStr, formatStr[1]);

    //Remove the conversion character from the format string
    formatStr = formatStr.substr(1);

    //If the value could not be read, exit the function
    if(value == "")
        return false;

    convertStrToValue(value, first);

    //If there are more values to be read, call the function again
    if(sizeof ... (others) > 0)
        return true && deSerialize(formatStr, valuesStr, others...);

    return true;
}


template<typename T>
void Serializer::convertStrToValue(const std::string& strValue, T& value)
{
    std::stringstream ss;
    ss << strValue;
    ss >> value;
}

#endif
