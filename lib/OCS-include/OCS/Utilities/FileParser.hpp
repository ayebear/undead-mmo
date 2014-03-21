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

#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <typeinfo>

struct Attribute
{
    std::string name;
    std::string value;

    int lineInFile;
    int lastLine;
};


class FileParser
{
 public:
        FileParser();
        FileParser(const std::string&, bool = true);
        ~FileParser();

        void setFilePath(const std::string&);
        std::string getFilePath() const;
        bool doesFileExist(const std::string&);
        bool createFile(const std::string&);

        template<typename T>
        bool createAttribute(const std::string&, const std::string&, const T&);
        bool createAttribute(const std::string&, const std::string&, const std::string&);
        bool createAttribute(const std::string&, const std::string&, bool);

        template<typename T>
        bool createAttribute(const std::string&, const std::string&, const std::vector<T>&, bool = false);
        bool createAttribute(const std::string&, const std::string&, const std::vector<std::string>&, bool = false);
        bool createAttribute(const std::string&, const std::string&, const std::vector<bool>&, bool = false);

        template<typename T>
        bool createAttribute(const std::string&, const std::string&, const std::vector<std::vector<T>>&, bool = true);
        bool createAttribute(const std::string&, const std::string&, const std::vector<std::vector<std::string>>&, bool = true);
        bool createAttribute(const std::string&, const std::string&, const std::vector<std::vector<bool>>&, bool = true);

        template<typename T>
        bool loadArray(const std::string&, const std::string&, std::vector<T>&, unsigned int = 0, std::vector<T> = {0});
        bool loadArray(const std::string&, const std::string&, std::vector<std::string>&, unsigned int = 0, std::vector<std::string> = {""});
        bool loadArray(const std::string&, const std::string&, std::vector<bool>&, unsigned int = 0, std::vector<bool> = {0});

        template<typename T>
        bool loadArray(const std::string&, const std::string&, std::vector<std::vector<T>>&);
        bool loadArray(const std::string&, const std::string&, std::vector<std::vector<std::string>>&);
        bool loadArray(const std::string&, const std::string&, std::vector<std::vector<bool>>&);

        template <typename T>
        bool setAttribute(const std::string&, const std::string&, const T&);
        bool setAttribute(const std::string&, const std::string&, const std::string&);
        bool setAttribute(const std::string&, const std::string&, bool);

        template<typename T>
        bool setAttribute(const std::string&, const std::string&, const std::vector<T>&, bool = false);
        bool setAttribute(const std::string&, const std::string&, const std::vector<std::string>&, bool = false);
        bool setAttribute(const std::string&, const std::string&, const std::vector<bool>&, bool = false);

        template<typename T>
        bool setAttribute(const std::string&, const std::string&, const std::vector<std::vector<T>>&, bool = false);
        bool setAttribute(const std::string&, const std::string&, const std::vector<std::vector<std::string>>&, bool = false);
        bool setAttribute(const std::string&, const std::string&, const std::vector<std::vector<bool>>&, bool = false);

        bool createSection(const std::string&);
        bool removeAttribute(const std::string&, const std::string&);
        bool removeSection(const std::string&);


        //Get value through section path and attribute name
        int getAttributeAsInt(const std::string&, const std::string&, int = 0);
        double getAttributeAsDecimal(const std::string&, const std::string&,  double = 0.0);
        std::string getAttributeAsString(const std::string&, const std::string&,  std::string = "");
        char getAttributeAsChar(const std::string&, const std::string&,  char = '\0');
        bool getAttributeAsBool(const std::string&, const std::string&,  bool = false);

        bool doesSectionExist(const std::string&);
        bool doesAttributeExist(const std::string&);

        template<typename T>
        T fromString(const std::string&);

    private:

        //Utility functions
        std::string buildAttributePath(const std::string&, const std::string&);
        std::vector<std::string> split(const std::string&, char, char = '\0', char = '\0');
        std::string trimWhiteSpace(const std::string&);
        std::string getSectionPathFromAttributePath(const std::string&);
        Attribute parseAttributePath(const std::string&, bool = false);
        Attribute searchSection(std::ifstream&, const std::string&, const std::string&, bool = false);
        Attribute searchDefaultSection(std::ifstream&, const std::string&, bool = false);
        bool isValidSectionPath(std::string);
        bool isValidAttributePath(std::string);
        int moveToSection(std::ifstream&, const std::string&);
        bool searchStringForEndBrace(std::string);
        bool writeLinesToFile(std::list<std::string>&);
        std::string removeOuterSymbols(const std::string&, char, char);
        int skipSection(std::ifstream&, const std::string&);
        bool isInsideDelimeters(const std::string&, unsigned int, char, char);
        void separateArrayValues(std::string, std::vector<std::string>&);
        void replaceAll(std::string&, const std::string&, const std::string&);

        std::string filePath;
};

/** \brief createAttribute()
 *             If the option does not exist, an option of the specified type is created and written to the end of the file.
 *             If the option already exists, nothing is changed.
 *
 * \param  optionName - The name of the option.
 * \param  value - The value of the option.
 * \return None
 *
 */
template<typename T>
bool FileParser::createAttribute(const std::string& sectionPath, const std::string& attributeName, const T& value)
{
    bool success = true;
    std::string attributePath = buildAttributePath(sectionPath, attributeName);
    //If the path was valid
    if(isValidAttributePath(attributePath))
    {
        //Only create the option if it does not exist.
        if(!doesAttributeExist(attributePath))
        {

                Attribute attribute;
                std::string sectionPath("");
                std::vector<std::string> elements = split(attributePath,'/');
                std::list<std::string> lines;

                //Attribute name is always the last element in the path
                attribute.name = elements.back();

        /*        if(typeid(value).name() == typeid(std::string("")).name())
                {
                    attribute.value = value;
                }
                else if(typeid(value).name() == typeid(bool).name())
                {
                    if(static_cast<bool>(value))
                        attribute.value = "true";
                    else
                        attribute.value = "false";
                }
                else*/
                {
                    std::stringstream ss;
                    ss << value;
                    attribute.value = ss.str();
                }

                //Remove the attribute name from the path
                elements.pop_back();

                //Build the section path
                for(int i = 0; i < elements.size(); ++i)
                {
                    if(i < elements.size() - 1)
                        sectionPath += trimWhiteSpace(elements[i]) + "/";
                    else
                        sectionPath += trimWhiteSpace(elements[i]);

                }

                //Create any sections that don't exist
                if(!doesSectionExist(sectionPath))
                {
                    std::cout << "Creating section " << sectionPath << std::endl;
                    createSection(sectionPath);

                }

            std::ifstream in(filePath);

            if(in.is_open())
            {
                int sectionLine = moveToSection(in, sectionPath);
                in.seekg(0, std::ios::beg);

                while(in.good())
                {
                    std::string currentLine;
                    std::getline(in , currentLine);
                    lines.push_back(currentLine);
                }

                auto it = lines.begin();
                //Move the iterator to the beginning of the first section that exists
                for(int i = 0; i < sectionLine; ++i)
                    ++it;

                bool endTagFound = false;
                std::string endTag = "[/" + removeOuterSymbols(elements.back(), '[', ']') + "]";

                while(it != lines.end() && !endTagFound)
                {
                    if(endTag == trimWhiteSpace(*it))
                        endTagFound = true;
                    ++it;
                }
                --it;
                std::string attributeLine("");

                for(int i = 0; i < elements.size(); ++i)
                    attributeLine += "\t";
                attributeLine += "\"" + attribute.name + "\" : \"" + attribute.value + "\"";
                lines.insert(it, attributeLine);
                writeLinesToFile(lines);

                in.close();
            }
            else
            {
                std::cerr << "Error opening file " << filePath << " in function createAttribute.\n";
                success = false;
            }

        }
        else
            std::cout << "Attribute " << attributePath << " already exists.\n";
    }
    else
    {
        std::cerr << "Invalid attribute path. Cannot create " << attributePath << std::endl;
        success = false;
    }

    return success;
}

template<typename T>
bool FileParser:: createAttribute(const std::string& sectionPath, const std::string& attributeName, const std::vector<T>& values, bool multiLine)
{
    bool success = true;
    std::string attributePath = buildAttributePath(sectionPath, attributeName);
    //If the path was valid
    if(isValidAttributePath(attributePath))
    {
        //Only create the option if it does not exist.
        if(!doesAttributeExist(attributePath))
        {

                Attribute attribute;
                std::string sectionPath("");
                std::vector<std::string> elements = split(attributePath,'/');
                std::list<std::string> lines;

                //Attribute name is always the last element in the path
                attribute.name = attribute.name;

                std::vector<std::string> attributeValues;

                for(unsigned int i = 0; i < values.size(); ++i)
                {
                    std::stringstream ss;
                    ss << values[i];
                    attributeValues.push_back(ss.str());
                    ss.clear();
                }
                //Remove the attribute name from the path
                elements.pop_back();

                //Build the section path
                for(int i = 0; i < elements.size(); ++i)
                {
                    if(i < elements.size() - 1)
                        sectionPath += trimWhiteSpace(elements[i]) + "/";
                    else
                        sectionPath += trimWhiteSpace(elements[i]);

                }

                //Create any sections that don't exist
                if(!doesSectionExist(sectionPath))
                {
                    std::cout << "Creating section " << sectionPath << std::endl;
                    createSection(sectionPath);
                    std::cout << "Created section " << sectionPath << std::endl;

                }

            std::ifstream in(filePath);

            if(in.is_open())
            {
                int sectionLine = moveToSection(in, sectionPath);
                in.seekg(0, std::ios::beg);

                while(in.good())
                {
                    std::string currentLine;
                    std::getline(in , currentLine);
                    lines.push_back(currentLine);
                }
                in.close();

                auto it = lines.begin();
                //Move the iterator to the beginning of the first section that exists
                for(int i = 0; i < sectionLine; ++i)
                    ++it;

                bool endTagFound = false;
                std::string endTag = "[/" + removeOuterSymbols(elements.back(), '[', ']') + "]";

                while(it != lines.end() && !endTagFound)
                {
                    if(endTag == trimWhiteSpace(*it))
                        endTagFound = true;
                    ++it;
                }
                --it;
                std::string attributeLine("");

                for(unsigned int i = 0; i < elements.size(); ++i)
                    attributeLine += "\t";
                attributeLine += "\"" + attribute.name + "\" : { ";
                if(multiLine)
                {
                    attributeLine += "\n";

                    for(unsigned int i = 0; i < elements.size() + 1; ++i)
                        attributeLine += "\t";
                }
                for(unsigned int i = 0; i < attributeValues.size(); ++i)
                {
                    if(i < attributeValues.size() - 1)
                        attributeLine += "\"" + attributeValues[i] +"\", ";
                    else
                        attributeLine += "\"" + attributeValues[i] +"\" ";

                    if(multiLine)
                    {
                        attributeLine += "\n";

                        for(unsigned int i = 0; i < elements.size() + 1; ++i)
                            attributeLine += "\t";
                    }
                }

                attributeLine += "}";
                lines.insert(it, attributeLine);
                writeLinesToFile(lines);


            }
            else
            {
                std::cerr << "Error opening file " << filePath << " in function createAttribute.\n";
                success = false;
            }

        }
        else
            std::cout << "Attribute " << attributePath << " already exists.\n";
    }
    else
    {
        std::cerr << "Invalid attribute path. Cannot create " << attributePath << std::endl;
        success = false;
    }

    return success;
}

template<typename T>
bool FileParser::createAttribute(const std::string& sectionPath, const std::string& attributeName, const std::vector<std::vector<T>>& values, bool multiLine)
{
    bool success = true;
    std::string attributePath = buildAttributePath(sectionPath, attributeName);
    //If the path was valid
    if(isValidAttributePath(attributePath))
    {
        //Only create the option if it does not exist.
        if(!doesAttributeExist(attributePath))
        {
                Attribute attribute;
                std::vector<std::string> elements = split(sectionPath,'/');
                std::list<std::string> lines;

                //Attribute name is always the last element in the path
                attribute.name = attributeName;

                std::vector<std::vector<std::string>> attributeValues;
                attributeValues.resize(values.size());
                for(unsigned int i = 0; i < values.size(); ++i)
                {
                    for(unsigned int j = 0; j < values[i].size(); ++j)
                    {
                        std::stringstream ss;
                        ss << values[i][j];
                        attributeValues[i].push_back(ss.str());
                        ss.clear();
                    }
                }

                //Create any sections that don't exist
                if(!doesSectionExist(sectionPath))
                {
                    std::cout << "Creating section " << sectionPath << std::endl;
                    createSection(sectionPath);
                    std::cout << "Created section " << sectionPath << std::endl;
                }

            std::ifstream in(filePath);

            if(in.is_open())
            {
                int sectionLine = moveToSection(in, sectionPath);
                in.seekg(0, std::ios::beg);

                while(in.good())
                {
                    std::string currentLine;
                    std::getline(in , currentLine);
                    lines.push_back(currentLine);
                }
                in.close();

                auto it = lines.begin();
                //Move the iterator to the beginning of the first section that exists
                for(int i = 0; i < sectionLine; ++i)
                    ++it;

                bool endTagFound = false;
                std::string endTag = "[/" + removeOuterSymbols(elements.back(), '[', ']') + "]";

                while(it != lines.end() && !endTagFound)
                {
                    if(endTag == trimWhiteSpace(*it))
                        endTagFound = true;
                    ++it;
                }
                --it;
                std::string attributeLine("");

                for(unsigned int i = 0; i < elements.size(); ++i)
                    attributeLine += "\t";
                attributeLine += "\"" + attribute.name + "\" : { ";
                if(multiLine)
                {
                    attributeLine += "\n";

                    for(unsigned int i = 0; i < elements.size() + 1; ++i)
                        attributeLine += "\t";
                }
                for(unsigned int i = 0; i < attributeValues.size(); ++i)
                {
                    //Create each inner array
                    attributeLine += "{ ";

                    for(unsigned int j = 0; j < attributeValues[i].size(); ++j)
                    {

                        if(j < attributeValues[i].size() - 1)
                            attributeLine += "\"" + attributeValues[i][j] +"\", ";
                        else
                            attributeLine += "\"" + attributeValues[i][j] +"\" ";

                    }
                    attributeLine += "} . ";

                    if(multiLine)
                    {
                        attributeLine += "\n";

                        for(unsigned int i = 0; i < elements.size() + 1; ++i)
                            attributeLine += "\t";
                    }
                }
                attributeLine += "}";
                lines.insert(it, attributeLine);
                writeLinesToFile(lines);
            }
            else
            {
                std::cerr << "Error opening file " << filePath << " in function createAttribute.\n";
                success = false;
            }

        }
        else
            std::cout << "Attribute " << attributePath << " already exists.\n";
    }
    else
    {
        std::cerr << "Invalid attribute path. Cannot create " << attributePath << std::endl;
        success = false;
    }

    return success;
}

template<typename T>
bool FileParser::loadArray(const std::string& sectionPath, const::std::string& attributeName, std::vector<T>& attributeValues, unsigned int expectedValues, std::vector<T> defaultArray)
{
    bool success = false;
    std::string path = buildAttributePath(sectionPath, attributeName);
    Attribute attribute = parseAttributePath(path, true);

    if(attribute.lineInFile != -1)
    {
        if(attribute.value[0] == '{')
        {
            T value;

            std::vector<std::string> values;

            attribute.value = removeOuterSymbols(attribute.value, '{', '}');
            values = split(attribute.value, ',');

            for(unsigned int i = 0; i < values.size(); ++i)
            {
                std::stringstream ss;
                values[i] = trimWhiteSpace(values[i]);
                values[i] = removeOuterSymbols(values[i], '"', '"');

                ss << values[i];
                ss >> value;
                if(values[i] != "")
                    attributeValues.push_back(value);

            }
            success = true;
        }
        else
            std::cerr << "Error: Attribute " << path << " is not an array.\n";
    }
    else
        std::cerr << "Error: Attribute " << path << " does not exist.\n";

    if(expectedValues > 0 && attributeValues.size() != expectedValues)
    {
        std::cerr << "Different number of elements than expected. Returning default array.\n";
        attributeValues = defaultArray;
        success = false;
    }


    return success;
}

template<typename T>
bool FileParser::loadArray(const std::string& sectionPath, const std::string& attributeName, std::vector<std::vector<T>>& attributeValues)
{

    bool success = false;
    std::string path = buildAttributePath(sectionPath, attributeName);
    Attribute attribute = parseAttributePath(path, true);

    if(attribute.lineInFile != -1)
    {
        if(attribute.value[0] == '{')
        {
            T value;

            std::vector<std::string> values;

            attribute.value = removeOuterSymbols(attribute.value, '{', '}');

            values = split(attribute.value, ',', '}', '{');
            attributeValues.resize(values.size());
            for(unsigned int i = 0; i < values.size(); ++i)
            {
                //std::cout << values[i] << std::endl;;
                std::vector<std::string> innerValues;
                separateArrayValues(values[i], innerValues);

                for(unsigned int j = 0; j < innerValues.size(); ++j)
                {
                    std::stringstream ss;
                    ss << innerValues[j];
                    ss >> value;
                    if(innerValues[j] != "")
                        attributeValues[i].push_back(value);
                }
                innerValues.clear();

            }
            success = true;
        }
        else
            std::cerr << "Error: Attribute " << path << " is not an array.\n";
    }
    else
        std::cerr << "Error: Attribute " << path << " does not exist.\n";

    return success;
}

/** \brief setAttribute()
 *             Changes the value or the type of the value of a given option.
 *             The entire file is read line by line into a vector and the line
 *                  that the option is on is modified to contain the new value.
 *             The file is then rewritten with the updated option.
 *             Order of all options is preserved.
 *             If the option does not exist, nothing is changed.
 *
 * \param  optionName - The name of the option to be changed.
 * \param  newValue - The value to replace the old value with
 * \return None
 *
 */

template<typename T>
bool FileParser::setAttribute(const std::string& sectionPath, const std::string& attributeName, const T& newValue)
{
    bool success = false;
    std::string attributePath = buildAttributePath(sectionPath, attributeName);
    Attribute attribute = parseAttributePath(attributePath);
    std::stringstream ss;
    ss << newValue;
    attribute.value = ss.str();

    if(attribute.lineInFile == -1)
    {
        std::cerr << "Attribute " << attributePath << " does not exist. Creating attribute.\n";
        success = createAttribute(sectionPath, attributeName, newValue);
    }
    else
    {
        std::list<std::string> lines;
        std::ifstream in(filePath);

        if(in.is_open())
        {
            while(in.good())
            {
                std::string currentLine;
                std::getline(in, currentLine);
                lines.push_back(currentLine);
            }
            in.close();
            auto it = lines.begin();

            for(int i = 0; i < attribute.lineInFile; ++i)
                ++it;
            std::string attributeLine("");
            std::vector<std::string> elements = split(attributePath, '/');

            for(int i = 0; i < elements.size() - 1; ++i)
                attributeLine += "\t";
            attributeLine += "\"" + attribute.name + "\" : \"" + attribute.value + "\"";
            *it = attributeLine;
            writeLinesToFile(lines);
            success = true;

        }
        else
            std::cerr << "Error opening file in 'setAttribute'\n";

    }

    return success;
}

template<typename T>
bool FileParser::setAttribute(const std::string& sectionPath, const std::string& attributeName, const std::vector<T>& values, bool multiLine)
{
    bool success = false;
    std::string attributePath = buildAttributePath(sectionPath, attributeName);
    Attribute attribute = parseAttributePath(attributePath, true);

    if(attribute.lineInFile == -1)
    {
        std::cerr << "Attribute " << attributePath << " does not exist. Creating attribute.\n";
        success = createAttribute(sectionPath, attributeName, values, multiLine);
    }
    else
    {
        std::list<std::string> lines;
        std::ifstream in(filePath);

        if(in.is_open())
        {
            while(in.good())
            {
                std::string currentLine;
                std::getline(in, currentLine);
                lines.push_back(currentLine);
            }
            in.close();

            auto beg = lines.begin();
            auto end = lines.begin();

            for(int i = 0; i < attribute.lineInFile; ++i)
                ++beg;
            for(int i = 0; i < attribute.lastLine + 1; ++i)
                ++end;

            lines.erase(beg, end);

            beg = lines.begin();
            end = lines.begin();

            for(int i = 0; i < attribute.lineInFile; ++i)
                ++beg;
            for(int i = 0; i < attribute.lastLine; ++i)
                ++end;


            std::string attributeLine("");
            std::vector<std::string> elements = split(attributePath, '/');

            for(unsigned int i = 0; i < elements.size() - 1; ++i)
                attributeLine += "\t";

            attributeLine += "\"" + attribute.name + "\" : { ";
            if(multiLine)
            {
                attributeLine += "\n";

                for(unsigned int i = 0; i < elements.size() + 1; ++i)
                    attributeLine += "\t";
            }
            for(unsigned int i = 0; i < values.size(); ++i)
            {
                std::stringstream ss;
                ss << values[i];

                if(i < values.size() - 1)
                    attributeLine += "\"" + ss.str() + "\", ";
                else
                    attributeLine += "\"" + ss.str() + "\" ";

                if(multiLine)
                {
                    attributeLine += "\n";

                    for(unsigned int i = 0; i < elements.size() + 1; ++i)
                        attributeLine += "\t";
                }
            }

            attributeLine += "}";
            lines.insert(beg, attributeLine);
            writeLinesToFile(lines);
            success = true;
        }
        else
            std::cerr << "Error opening file in 'setAttribute'\n";

    }

    return success;
}

template<typename T>
bool FileParser::setAttribute(const std::string& sectionPath, const std::string& attributeName, const std::vector<std::vector<T>>& values, bool multiLine)
{
    bool success = false;
    std::string attributePath = buildAttributePath(sectionPath, attributeName);
    Attribute attribute = parseAttributePath(attributePath, true);

    if(attribute.lineInFile == -1)
    {
        std::cerr << "Attribute " << attributePath << " does not exist. Creating attribute.\n";
        success = createAttribute(sectionPath, attributeName, values, multiLine);
    }
    else
    {
        std::list<std::string> lines;
        std::ifstream in(filePath);

        if(in.is_open())
        {

            std::vector<std::vector<std::string>> attributeValues;
            attributeValues.resize(values.size());
            for(unsigned int i = 0; i < values.size(); ++i)
            {
                for(unsigned int j = 0; j < values[i].size(); ++j)
                {
                    std::stringstream ss;
                    ss << values[i][j];
                    attributeValues[i].push_back(ss.str());
                    ss.clear();
                }
            }

            while(in.good())
            {
                std::string currentLine;
                std::getline(in, currentLine);
                lines.push_back(currentLine);
            }
            in.close();

            auto beg = lines.begin();
            auto end = lines.begin();

            for(int i = 0; i < attribute.lineInFile; ++i)
                ++beg;
            for(int i = 0; i < attribute.lastLine + 1; ++i)
                ++end;

            lines.erase(beg, end);

            beg = lines.begin();
            end = lines.begin();

            for(int i = 0; i < attribute.lineInFile; ++i)
                ++beg;
            for(int i = 0; i < attribute.lastLine; ++i)
                ++end;

            std::vector<std::string> elements = split(sectionPath, '/');
            std::string attributeLine("");

            for(unsigned int i = 0; i < elements.size(); ++i)
                attributeLine += "\t";
            attributeLine += "\"" + attribute.name + "\" : { ";
            if(multiLine)
            {
                attributeLine += "\n";

                for(unsigned int i = 0; i < elements.size() + 1; ++i)
                    attributeLine += "\t";
            }
            for(unsigned int i = 0; i < attributeValues.size(); ++i)
            {
                //Create each inner array
                attributeLine += "{ ";

                for(unsigned int j = 0; j < attributeValues[i].size(); ++j)
                {

                    if(j < attributeValues[i].size() - 1)
                        attributeLine += "\"" + attributeValues[i][j] +"\", ";
                    else
                        attributeLine += "\"" + attributeValues[i][j] +"\" ";

                }
                attributeLine += "} . ";

                if(multiLine)
                {
                    attributeLine += "\n";

                    for(unsigned int i = 0; i < elements.size() + 1; ++i)
                        attributeLine += "\t";
                }
            }
            attributeLine += "}";
            lines.insert(beg, attributeLine);
            writeLinesToFile(lines);
            success = true;
        }
        else
            std::cerr << "Error opening file in 'setAttribute'\n";

    }

    return success;
}

template<typename T>
T FileParser::fromString(const std::string& str)
{
    T convertedValue;
    std::stringstream ss(str);
    ss >> convertedValue;

    return convertedValue;
}

#endif // FILEPARSER_H
