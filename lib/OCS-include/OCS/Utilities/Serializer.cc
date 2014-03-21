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

#include "Serializer.hpp"
#include <iostream>

/** \brief Store everything in a stringstream instead of the string up to the first instance of whitespace
 *
 * \param in The stringstream with the string to extract
 * \param str The string to extract to
 * \return The stringstream that was passed in.
 *
 */
std::stringstream& operator>>(std::stringstream& in, std::string& str)
{
    str = in.str();
    return in;
}

std::stringstream& operator<<(std::stringstream& out, std::string& str)
{
    out.str() = str;
    return out;
}

const std::set<std::string> Serializer::escapableCharacters = {"%", "'"};

/** \brief Separate a string with multiple values and strip off the first value by searching for the
 *         specified terminating character and splitting the string into two parts.
 *         The beginning of the source string should be the beginning of the first value.
 *
 * \param source The string with the list of values
 * \param terminatingChar The character that indicates the end of the value
 *
 * \return A string containing the first value
 *
 */
std::string Serializer::getNextValue(std::string& source, char terminatingChar)
{
    std::string value("");
    unsigned int idx = 0;
    bool endFound = false;

    //Start at the beginning of the string and copy each character to a new value string until the end of
    //the source string or the first value is reached.
    auto it = source.begin();
    while(it != source.end() && !endFound)
    {
        //Copy the current character and advance to the next one
        value += *it;
        ++idx;
        ++it;

        //If the current character is the terminating character
        if(*it == terminatingChar)
        {
            //Check if the terminating character is not escaped
            if(*(it - 1) == escapeChar)
            {
                //Check if the previous escape character is itself escaped
                if(*(it - 2) == escapeChar)
                    endFound = true;
            }
            else
                endFound = true;
        }
    }
    //If the last character was the terminating character then decrement it.
    //If the last value is at the end of the string, the value wasn't being read, so this is a quick fix until I figure that out.
    if(endFound)
        --it;

    //If the end of the source string was reached, then there was no terminating character.
    if(it == source.end())
    {
        std::cerr << "Error: Reached end of source string '" << source << "' without finding terminating character '" << terminatingChar << "'\n";
        return("");
    }

    //Remove any escaped characters from the value itself
    removeAllEscapeChars(value);

    //Strip off the first value in the source string
    source = source.substr(idx);

    return value;
}

/** \brief Search a format string for the next conversion character. Also keep track of the index in the values string
 *         to keep them aligned.
 *
 * \param frmtString The format string that indicates how values should be read.
 * \param valuesString the string containing the values
 * \return True if the conversion character was found and the values string matched the format string.
 *         False if there were no more conversion characters or the values string did not match the format string.
 *
 */
bool Serializer::moveToNextConversionChar(std::string& frmtString, std::string& valuesString)
{
    unsigned int frmtIndex = 0;
    unsigned int valuesIndex = 0;

    //Iterate through both strings until either the end of one of the strings is reached or the conversion character is found.
    while(frmtIndex < frmtString.size() && valuesIndex < valuesString.size() && frmtString[frmtIndex] != conversionChar)
    {

        if(frmtString[frmtIndex] != valuesString[valuesIndex])
        {
            std::cerr << "Error: Format string '" << frmtString << "' does not match values string '" << valuesString << "'\n";
            return false;
        }

        ++frmtIndex;
        ++valuesIndex;
    }

    if(frmtIndex >= frmtString.size() || valuesIndex >= valuesString.size())
    {
        std::cerr << "Error: End of strings reached without finding conversion character.\n";
        return false;
    }

    //Strip everything before the conversion character in both strings
    frmtString = frmtString.substr(frmtIndex);
    valuesString = valuesString.substr(valuesIndex);

    return true;

}

/** \brief Remove all escape characters that were escaping another character.
 *
 * \param str The string that contains escaped characters
 *
 */
void Serializer::removeAllEscapeChars(std::string& str)
{
    auto idx = str.find(escapeChar);

    //While there are more escape characters to remove
    while(idx != std::string::npos)
    {
        //Get the character right after the escape character
        std::string nextChar(str.substr(idx + 1, 1));

        //If the next character is in the list of valid escape characters
        if(escapableCharacters.find(nextChar) != escapableCharacters.end())
        {
            //Split the list into two parts.
            //The beginning includes everything up to but not including the escape character
            std::string beginning = str.substr(0, idx);

            //The end starts at the character right after the escape character
            std::string ending = str.substr(idx + 1);

            std::swap(beginning, str);
            str += ending;
        }

        //Look for more escape characters in the rest of the string
        idx = str.find(escapeChar, idx + 1);
    }

}

/** \brief Add any necessary escape characters to the string so it can be deserialized properly.
 *
 * \param str The string to add escape characters to.
 *
 */
void Serializer::addEscapeCharacters(std::string& str)
{

    //Iterate through all escapable characters
    for(const auto& i : escapableCharacters)
    {
        auto idx = str.find(i);

        //While there are more escape characters to add
        while(idx != std::string::npos)
        {
            //Split the list into two parts.
            //The beginning includes everything up to but not including the escapable character
            std::string beginning = str.substr(0, idx);

            //The end starts at the escapable character
            std::string ending = str.substr(idx);

            std::swap(beginning, str);
            str += escapeChar + ending;

            //Look for more escapable characters in the rest of the string.
            //idx is now at the escape character. idx + 1 is the last escapable character. We want to start searching
            //immediately after the last escapable character.
            idx = str.find(i, idx + 2);
        }
    }
}
