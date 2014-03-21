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

#include "FileParser.hpp"
#include <stdexcept>
#include <ctime>

FileParser::FileParser()
{
    //ctor
}

FileParser::FileParser(const std::string& fPath, bool createIfDoesNotExist) :
    filePath(fPath)
{
    if(createIfDoesNotExist)
        createFile(filePath);
}

FileParser::~FileParser()
{
    //dtor

}

void FileParser::setFilePath(const std::string& fPath)
{
    filePath = fPath;
}

std::string FileParser::getFilePath() const
{
    return filePath;
}

bool FileParser::doesFileExist(const std::string& path)
{
    bool exists = false;
    std::ifstream in(path);

    if(in.is_open())
    {
        exists = true;
        in.close();
    }

    return exists;
}

bool FileParser::createFile(const std::string& path)
{
    bool success = false;
    if(!doesFileExist(path))
    {
        std::ofstream out(path);

        if(out.is_open())
        {
            time_t currentTime = time(0);
            struct tm tStruct = *localtime(&currentTime);
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%Y-%b-%d at %I:%M:%S %p", &tStruct);
            success = true;
            out << "#File created on " << buffer << "\n";
            out.close();
            filePath = path;
        }
        else
            std::cerr << "Error creating file " << path << ".\n";
    }
    //else
        //std::cerr << "File " << path << " already exists.\n";

    return success;
}

bool FileParser::createAttribute(const std::string& sectionPath, const std::string& attributeName, const std::string& value)
{
    bool success = true;
    std::string attributePath = buildAttributePath(sectionPath, attributeName);
    //If the path was valid
    if(isValidAttributePath(attributePath))
    {
        //Only create the option if it does not exist.
        if(!doesAttributeExist(attributePath))
        {
                std::list<std::string> lines;

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
                std::vector<std::string> elements = split(sectionPath, '/');
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
                attributeLine += "\"" + attributeName + "\" : \"" + value + "\"";
                lines.insert(it, attributeLine);
                writeLinesToFile(lines);

                in.close();
            }
            else
            {
                std::cerr << "Error opening file " << filePath << " in function createAttribute\n";
                success = false;
            }

        }
        else
            std::cout << "Attribute " << attributePath << " already exists\n";
    }
    else
    {
        std::cerr << "Invalid attribute path. Cannot create " << attributePath << std::endl;
        success = false;
    }

    return success;
}

bool FileParser::createAttribute(const std::string& sectionPath, const std::string& attributeName, bool value)
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

                if(value)
                    attribute.value = "true";
                else
                    attribute.value = "false";

                //Remove the attribute name from the path
                elements.pop_back();

                //Build the section path
                for(unsigned int i = 0; i < elements.size(); ++i)
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

                for(unsigned int i = 0; i < elements.size(); ++i)
                    attributeLine += "\t";
                attributeLine += "\"" + attribute.name + "\" : \"" + attribute.value + "\"";
                lines.insert(it, attributeLine);
                writeLinesToFile(lines);

                in.close();
            }
            else
            {
                std::cerr << "Error opening file " << filePath << " in function createAttribute\n";
                success = false;
            }

        }
        else
            std::cout << "Attribute " << attributePath << " already exists\n";
    }
    else
    {
        std::cerr << "Invalid attribute path. Cannot create " << attributePath << std::endl;
        success = false;
    }

    return success;
}

bool FileParser::createAttribute(const std::string& sectionPath, const std::string& attributeName, const std::vector<std::string>& values, bool multiLine)
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

                std::vector<std::string> attributeValues;

                for(unsigned int i = 0; i < values.size(); ++i)
                {
                    attributeValues.push_back(values[i]);
                }
                //Remove the attribute name from the path
                elements.pop_back();

                //Build the section path
                for(unsigned int i = 0; i < elements.size(); ++i)
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

bool FileParser::createAttribute(const std::string& sectionPath, const std::string& attributeName, const std::vector<bool>& values, bool multiLine)
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

                std::vector<std::string> attributeValues;

                for(unsigned int i = 0; i < values.size(); ++i)
                {
                    if(values[i])
                        attributeValues.push_back("true");
                    else
                        attributeValues.push_back("false");
                }
                //Remove the attribute name from the path
                elements.pop_back();

                //Build the section path
                for(unsigned int i = 0; i < elements.size(); ++i)
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
                    attributeLine += "\"" + attributeValues[i] + "\"";

                    if(i < attributeValues.size() - 1)
                        attributeLine += ", ";
                    else
                        attributeLine += " ";

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

bool FileParser::createAttribute(const std::string& sectionPath, const std::string& attributeName, const std::vector<std::vector<std::string>>& values, bool multiLine)
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
                        attributeValues[i].push_back(values[i][j]);
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

bool FileParser::createAttribute(const std::string& sectionPath, const std::string& attributeName, const std::vector<std::vector<bool>>& values, bool multiLine)
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
                        if(values[i][j])
                            attributeValues[i].push_back("true");
                        else
                            attributeValues[i].push_back("false");
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


bool FileParser::setAttribute(const std::string& sectionPath, const std::string& attributeName, const std::vector<std::string>& values, bool multiLine)
{
    bool success = false;
    std::string attributePath = buildAttributePath(sectionPath, attributeName);
    Attribute attribute = parseAttributePath(attributePath, true);

    if(attribute.lineInFile == -1)
    {
        std::cerr << "Attribute " << attributePath << " does not exist.\nCreating attribute.\n";
        success = createAttribute(sectionPath, attributeName, values);
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

                if(i < values.size() - 1)
                    attributeLine += "\"" + values[i] + "\", ";
                else
                    attributeLine += "\"" + values[i] + "\" ";

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

bool FileParser::setAttribute(const std::string& sectionPath, const std::string& attributeName, const std::vector<bool>& values, bool multiLine)
{
    bool success = false;
    std::string attributePath = buildAttributePath(sectionPath, attributeName);
    Attribute attribute = parseAttributePath(attributePath, true);

    if(attribute.lineInFile == -1)
    {
        std::cerr << "Attribute " << attributePath << " does not exist.\nCreating attribute.\n";
        success = createAttribute(sectionPath, attributeName, values);
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

                attributeLine += "\"";

                if(values[i])
                    attributeLine += "true";
                else
                    attributeLine += "false";

                if(i < values.size() - 1)
                    attributeLine += "\", ";
                else
                    attributeLine += "\" ";

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

bool FileParser::setAttribute(const std::string& sectionPath, const std::string& attributeName, const std::string& newValue)
{
    std::string attributePath = buildAttributePath(sectionPath, attributeName);
    bool success = false;
    Attribute attribute = parseAttributePath(attributePath);

    attribute.value = newValue;

    if(attribute.lineInFile == -1)
    {
        std::cerr << "Attribute " << attributePath << " does not exist.\nCreating attribute.\n";
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

            auto it = lines.begin();

            for(int i = 0; i < attribute.lineInFile; ++i)
                ++it;
            std::string attributeLine("");
            std::vector<std::string> elements = split(attributePath, '/');

            for(unsigned int i = 0; i < elements.size() - 1; ++i)
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

bool FileParser::setAttribute(const std::string& sectionPath, const std::string& attributeName, bool newValue)
{
    std::string attributePath = buildAttributePath(sectionPath, attributeName);
    bool success = false;
    Attribute attribute = parseAttributePath(attributePath);

    if(newValue)
        attribute.value = "true";
    else
        attribute.value = "false";

    if(attribute.lineInFile == -1)
    {
        std::cerr << "Attribute " << attributePath << " does not exist.\nCreating attribute.\n";
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

            auto it = lines.begin();

            for(int i = 0; i < attribute.lineInFile; ++i)
                ++it;
            std::string attributeLine("");
            std::vector<std::string> elements = split(attributePath, '/');

            for(unsigned int i = 0; i < elements.size() - 1; ++i)
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

bool FileParser::setAttribute(const std::string& sectionPath, const std::string& attributeName, const std::vector<std::vector<std::string>>& values, bool multiLine)
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
                    attributeValues[i].push_back(values[i][j]);
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

bool FileParser::setAttribute(const std::string& sectionPath, const std::string& attributeName, const std::vector<std::vector<bool>>& values, bool multiLine)
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
                    if(values[i][j])
                        attributeValues[i].push_back("true");
                    else
                        attributeValues[i].push_back("false");
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

bool FileParser::removeAttribute(const std::string& sectionPath, const std::string& attributeName)
{
    std::string path = buildAttributePath(sectionPath, attributeName);
    Attribute attribute = parseAttributePath(path);
    if(attribute.value[0] == '{')
        attribute = parseAttributePath(path, true);
    bool success = false;

    if(attribute.lineInFile != -1)
    {
        std::ifstream in(filePath);

        if(in.is_open())
        {
            std::list<std::string> lines;
            std::string currentLine;

            while(in.good())
            {
                std::getline(in, currentLine);
                lines.push_back(currentLine);
            }

            auto it = lines.begin();
            auto end = lines.begin();

            for(int i = 0; i < attribute.lineInFile; ++i)
                ++it;

            for(int i = 0; i < attribute.lastLine + 1; ++i)
                ++end;

            lines.erase(it, end);

            writeLinesToFile(lines);

            in.close();

            success = true;
        }
        else
            std::cerr << "Error opening file in 'removeAttribute.\n";
    }
    else
        std::cerr << "Attribute " << path << " does not exist.\n";

    return success;
}

void FileParser::separateArrayValues(std::string str, std::vector<std::string>& values)
{

    bool startQuotesFound = false;
    int startQuotePos = 0;
    std::string valueString("");
    str = trimWhiteSpace(str);
    //std::cout << str << std::endl;

    for(unsigned int i = 0; i < str.size(); ++i)
    {
        if(str[i] == '"' &&!startQuotesFound)
        {
            if(i > 0 && str[i - 1] != '\\')
            {
                startQuotesFound = true;
                startQuotePos = i;
                //std::cout << "Found start quote at " << i << std::endl;
            }
            else if(i == 0)
            {
                startQuotesFound = true;
                startQuotePos = i;
               // std::cout << "Found start quote at " << i << std::endl;
            }
        }
        else if(str[i] == '"' && startQuotesFound)
        {
            if(i > 0 && str[i - 1] != '\\')
            {
                startQuotesFound = false;
               // std::cout << "Found end quote at " << i << std::endl;
                valueString = str.substr(startQuotePos + 1, i - startQuotePos - 1);

                replaceAll(valueString, "\\\"", "\"");
                replaceAll(valueString, "\\n", "\n");
                replaceAll(valueString, "\\t", "\t");
                //std::cout << valueString;
                values.push_back(valueString);
            }
        }
    }
}

void FileParser::replaceAll(std::string& str, const std::string& from, const std::string& to)
{
    if(from.empty())
        return;

    std::string ret;
    ret.reserve(str.length());
    std::size_t start = 0;
    std::size_t pos;
    while((pos = str.find(from, start)) != std::string::npos)
    {
        ret += str.substr(start, pos - start);
        ret += to;
        pos += from.length();
        start = pos;
    }
    ret += str.substr(start);
    str.swap(ret);
}

bool FileParser::loadArray(const std::string& sectionPath, const std::string& attributeName, std::vector<std::string>& attributeValues, unsigned int expectedValues, std::vector<std::string> defaultArray)
{
    bool success = false;
    std::string path = buildAttributePath(sectionPath, attributeName);

    Attribute attribute = parseAttributePath(path, true);

    if(attribute.lineInFile != -1)
    {
        if(attribute.value[0] == '{')
        {
            std::vector<std::string> values;

            attribute.value = removeOuterSymbols(attribute.value, '{', '}');
            //std::cout << attribute.value << std::endl;
           // values = split(attribute.value, ',', '"', '"');
            separateArrayValues(attribute.value, values);

            for(unsigned int i = 0; i < values.size(); ++i)
            {
                if(values[i] != "")
                {
                    //std::cout << values[i]<< std::endl;
                    attributeValues.push_back(values[i]);
                }
            }
            //std::cout << "\n\n";
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

bool FileParser::loadArray(const std::string& sectionPath, const std::string& attributeName, std::vector<bool>& attributeValues, unsigned int expectedValues, std::vector<bool> defaultArray)
{
    bool success = false;
    std::string path = buildAttributePath(sectionPath, attributeName);
    Attribute attribute = parseAttributePath(path, true);

    if(attribute.lineInFile != -1)
    {
        if(attribute.value[0] == '{')
        {
            std::vector<std::string> values;
            attribute.value = removeOuterSymbols(attribute.value, '{', '}');
            separateArrayValues(attribute.value, values);

            for(unsigned int i = 0; i < values.size(); ++i)
            {
                try
                {
                    int val = fromString<int>(values[i]);

                    if(val <= 0)
                        attributeValues.push_back(false);
                    else
                        attributeValues.push_back(true);

                }

                catch(std::invalid_argument& ex)
                {

                    std::string value = values[i];
                    for(auto& i : value)
                        i = tolower(i);
                    if(value == "true")
                        attributeValues.push_back(true);
                    else if(value == "false")
                        attributeValues.push_back(false);
                    else
                        std::cerr << "Error parsing value for " << attribute.name << " as a boolean.\nCheck attribute value to ensure value is in proper format.\n";
                }

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

bool FileParser::loadArray(const std::string& sectionPath, const std::string& attributeName, std::vector<std::vector<std::string>>& attributeValues)
{

    bool success = false;
    std::string path = buildAttributePath(sectionPath, attributeName);
    Attribute attribute = parseAttributePath(path, true);

    if(attribute.lineInFile != -1)
    {
        if(attribute.value[0] == '{')
        {
            std::string value;

            std::vector<std::string> values;

            attribute.value = removeOuterSymbols(attribute.value, '{', '}');

            values = split(attribute.value, ',', '}', '{');
            attributeValues.resize(values.size());
            for(unsigned int i = 0; i < values.size(); ++i)
            {
                std::vector<std::string> innerValues;
                separateArrayValues(values[i], innerValues);

                for(unsigned int j = 0; j < innerValues.size(); ++j)
                {
                    if(innerValues[j] != "")
                        attributeValues[i].push_back(innerValues[j]);
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

bool FileParser::loadArray(const std::string& sectionPath, const std::string& attributeName, std::vector<std::vector<bool>>& attributeValues)
{

    bool success = false;
    std::string path = buildAttributePath(sectionPath, attributeName);
    Attribute attribute = parseAttributePath(path, true);
//std::cout << "Value " << attribute.value << std::endl;
    if(attribute.lineInFile != -1)
    {
        if(attribute.value[0] == '{')
        {

            std::vector<std::string> values;

            attribute.value = removeOuterSymbols(attribute.value, '{', '}');

            values = split(attribute.value, ',', '}', '{');
            attributeValues.resize(values.size());
            for(unsigned int i = 0; i < values.size(); ++i)
            {
                std::vector<std::string> innerValues;
                separateArrayValues(values[i], innerValues);
                for(unsigned int j = 0; j < innerValues.size(); ++j)
                {
                    try
                    {
                        int val = fromString<int>(innerValues[j]);

                        if(val <= 0)
                            attributeValues[i].push_back(false);
                        else
                            attributeValues[i].push_back(true);

                    }

                    catch(std::invalid_argument& ex)
                    {

                        std::string value = innerValues[j];
                        for(auto& i : value)
                            i = tolower(i);
                        if(value == "true")
                            attributeValues[i].push_back(true);
                        else if(value == "false")
                            attributeValues[i].push_back(false);
                        else
                            std::cerr << "Error parsing value for " << attribute.name << " as a boolean.\nCheck attribute value to ensure value is in proper format.\n";
                    }
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

bool FileParser::removeSection(const std::string& path)
{
    bool success = false;
    std::ifstream in(filePath);

    if(in.is_open())
    {
        int sectionBeginning = moveToSection(in, path);

        if(sectionBeginning != -1)
        {
            std::string lastSection = split(path, '/').back();
            int sectionEnd = sectionBeginning + skipSection(in, lastSection);
            in.seekg(0, std::ios::beg);
            std::list<std::string> lines;
            std::string currentLine;

            while(in.good())
            {
                std::getline(in, currentLine);
                lines.push_back(currentLine);
            }
            auto beg = lines.begin();
            auto end = lines.begin();

            for(int i = 0; i < sectionBeginning; ++i)
                ++beg;
            for(int i = 0; i < sectionEnd; ++i)
                ++end;

            lines.erase(beg, end);

            writeLinesToFile(lines);
        }
        else
            std::cerr << "Section " << path << " does not exist.\n";
    }
    else
        std::cerr << "Error opening file in 'removeSection.\n";

    return success;
}


//Get value through section path and attribute name
int FileParser::getAttributeAsInt(const std::string& sectionPath, const std::string& attributeName, int defaultValue)
{
    std::string attributePath = buildAttributePath(sectionPath, attributeName);
    Attribute attribute = parseAttributePath(attributePath);

    if(attribute.lineInFile != -1)
    {
        try
        {
            return fromString<int>(attribute.value);
        }

        catch(std::invalid_argument& ex)
        {
            std::cerr << "Error parsing value for " << attribute.name << " as an integer. Returning '" << defaultValue << "'.\n";
        }
    }
    else
        std::cerr << "Attribute " << attributePath << " does not exist. Returning '" << defaultValue << "'\n";

    return defaultValue;
}

double FileParser::getAttributeAsDecimal(const std::string& sectionPath, const std::string& attributeName,  double defaultValue)
{
    std::string attributePath = buildAttributePath(sectionPath, attributeName);
    Attribute attribute = parseAttributePath(attributePath);

    if(attribute.lineInFile != -1)
    {
        try
        {
            return fromString<double>(attribute.value);
        }

        catch(std::invalid_argument& ex)
        {
            std::cerr << "Error parsing value for " << attribute.name << " as a decimal. Returning '" << defaultValue << "'.\n";
        }
    }
    else
        std::cerr << "Attribute " << attributePath << " does not exist. Returning '" << defaultValue << "'\n";

    return defaultValue;
}

std::string FileParser::getAttributeAsString(const std::string& sectionPath, const std::string& attributeName,  std::string defaultValue)
{
    std::string attributePath = buildAttributePath(sectionPath, attributeName);
    Attribute attribute = parseAttributePath(attributePath);

    if(attribute.lineInFile != -1)
    {
        try
        {
            return attribute.value;
        }

        catch(std::invalid_argument& ex)
        {
            std::cerr << "Error parsing value for " << attribute.name << " as a string. Returning '" << defaultValue << "'.\n";
        }
    }
    else
        std::cerr << "Attribute " << attributePath << " does not exist. Returning '" << defaultValue << "'\n";

    return defaultValue;
}

char FileParser::getAttributeAsChar(const std::string& sectionPath, const std::string& attributeName,  char defaultValue)
{
    std::string attributePath = buildAttributePath(sectionPath, attributeName);
    Attribute attribute = parseAttributePath(attributePath);
    if(attribute.lineInFile != -1)
    {
        try
        {
            return attribute.value[0];
        }

        catch(std::invalid_argument& ex)
        {
            std::cerr << "Error parsing value for " << attribute.name << " as a character. Returning '" << defaultValue << "'.\n";
        }
    }
    else
        std::cerr << "Attribute " << attributePath << " does not exist. Returning '" << defaultValue << "'\n";

    return defaultValue;
}

bool FileParser::getAttributeAsBool(const std::string& sectionPath, const std::string& attributeName,  bool defaultValue)
{
    std::string attributePath = buildAttributePath(sectionPath, attributeName);
    Attribute attribute = parseAttributePath(attributePath);

    if(attribute.lineInFile != -1)
    {
        try
        {
            int val = fromString<int>(attribute.value);

            if(val <= 0)
                return false;
            else
                return true;

        }

        catch(std::invalid_argument& ex)
        {

            std::string value = attribute.value;
            for(auto& i : value)
                i = tolower(i);
            if(value == "true")
                return true;
            else if(value == "false")
                return false;
            else
                std::cerr << "Error parsing value for " << attribute.name << " as a boolean. Returning '" << defaultValue << "'.\n";
        }
    }
    else
        std::cerr << "Attribute " << attributePath << " does not exist. Returning '" << defaultValue << "'\n";

    return defaultValue;
}

std::string FileParser::buildAttributePath(const std::string& sectionPath, const std::string& attributeName)
{
    return sectionPath + "/" + attributeName;
}

bool FileParser::createSection(const std::string& sectionPath)
{
    std::ifstream in(filePath);
    bool sectionExists = false;

    if(in.is_open())
    {
        //Name of each section
        std::vector<std::string> sectionHeaders = split(sectionPath, '/');
        std::vector<std::string> sectionEndTags;

        for(auto sectionName : sectionHeaders)
            sectionEndTags.push_back("[/" + removeOuterSymbols(sectionName, '[', ']') + "]");

        //Line number of each section that exists
        std::vector<int> sectionLines(sectionHeaders.size(), -1);

        unsigned int currentSectionIndex = 0;
        int currentLineNumber = -1;

        //All lines in the file
        std::list<std::string> lines;
        bool outerEndTagFound = false;

        //Read all lines from the file and store the line numbers of all existing sections
        while(in.good())
        {
            //std::cout << currentSectionIndex << ", " << sectionHeaders.size() << std::endl;
            std::string currentOuterTag = sectionEndTags[currentSectionIndex];
            std::string currentLine;
            std::string trimmedLine;
            std::getline(in, currentLine);
            ++currentLineNumber;

            trimmedLine = trimWhiteSpace(currentLine);

            lines.push_back(currentLine);

            if(trimmedLine == currentOuterTag)
                outerEndTagFound = true;

            if(trimmedLine == sectionHeaders[currentSectionIndex] && !outerEndTagFound)
            {
                sectionLines[currentSectionIndex] = currentLineNumber;
                outerEndTagFound = false;
                ++currentSectionIndex;

                if(currentSectionIndex == sectionEndTags.size())
                {
                    sectionExists = true;
                    --currentSectionIndex;
                }
            }

        }
        in.close();

        auto it = lines.begin();
        unsigned int lastCreatedSection = 0;
        unsigned int firstUncreated = 0;

        while(lastCreatedSection < sectionLines.size() && sectionLines[lastCreatedSection] != -1)
        {
            ++lastCreatedSection;
            ++firstUncreated;
        }


        if(lastCreatedSection > 0)
            --lastCreatedSection;

        //Move the iterator to the beginning of the first section that exists
        for(int i = 0; i < sectionLines[lastCreatedSection]; ++i)
            ++it;

        bool foundEndTag = false;
        //Move the iterator to the end of the last existing section
        while(!foundEndTag && it != lines.end())
        {
            std::string trimmedLine = trimWhiteSpace(*it);

            if(trimmedLine == sectionEndTags[lastCreatedSection])
                foundEndTag = true;
            ++it;
        }
        --it;

        //Create each section
        for(unsigned int nextSection = firstUncreated; nextSection < sectionHeaders.size(); ++nextSection)
        {
            std::string headerTag("");

            for(unsigned int i = 0; i < nextSection; ++i)
                headerTag += "\t";

            headerTag += sectionHeaders[nextSection];
            it = lines.insert(it, headerTag);
            ++it;
        }

        //Create all section end tags
        unsigned int nextEndTag = sectionEndTags.size() - 1;
        do
        {
            //std::cout << sectionEndTags[nextEndTag] << std::endl;
            std::string endTag("");
            for(unsigned int j = 0; j < nextEndTag; ++j)
                endTag += "\t";
            endTag += sectionEndTags[nextEndTag];

            it = lines.insert(it, endTag);
            ++it;
            --nextEndTag;
        }while(nextEndTag >= firstUncreated && nextEndTag >= 0);

        writeLinesToFile(lines);

    }
    else
    {
        std::cerr << "Error opening file " << filePath << " in function 'createSection'\n";
        return false;
    }

    if(sectionExists)
    {
        std::cout << "A section already exists with the path " << sectionPath << std::endl;
    }

    return true;
}

bool FileParser::writeLinesToFile(std::list<std::string>& lines)
{
    std::ofstream out(filePath);

    if(out.is_open())
    {

        auto it = lines.begin();
        while(it != lines.end())
        {

            if(trimWhiteSpace(*it) != "")
                out << *it << "\n";
            ++it;
        }

        out.close();
        return true;
    }
    else
    {
        std::cerr << "Error opening file " << filePath << " for writing\n";
        return false;
    }
}

/** \brief doesOptionExist()
 *             Checks if a given option exists in the file.
 *
 * \param  optionName - The name of the option to find.
 * \return returns true if the option exists, otherwise it returns false.
 *
 */
bool FileParser::doesAttributeExist(const std::string& attributePath)
{
    return parseAttributePath(attributePath).lineInFile != -1;
}

bool FileParser::doesSectionExist(const std::string& sectionPath)
{
    std::ifstream in(filePath);

    if(in.is_open())
    {
        bool exists = moveToSection(in, sectionPath) != -1;
        in.close();
        return exists;
    }
    else
    {
        std::cerr << "Error opening file " << filePath << " in function 'doesSectionExist'\n";
        return false;
    }
}

//Utility functions

/** \brief split()
 *            Splits a string into separate parts based on a specific character
 *
 * \param  str - The string to split
 * \param  separator - The character to split the strings by
 * \return A vector with the sub-string before the separator and the sub-string after the separator
  *           If there is no ocurrence of the separator, a vector with the original string is returned.
 *
 * \todo      Make the function split the string multiple times if there are multiple separators.
 *            Currently, the function only splits a string into two parts. If there are multiple separators
 *            then the second sub-string will contain the remaining separators.
 *
 */
std::vector<std::string> FileParser::split(const std::string& str, char separator, char leftDelim, char rightDelim)
{
    std::string tempStr = str;
    std::vector<std::string> substrings;
    std::size_t index = tempStr.find_first_of(separator);
    //While the separator was in the string
    while(index != std::string::npos)
    {
        if(leftDelim == '\0' && rightDelim == '\0')
        {
            //Take the sub string before the separator and store it.
            substrings.push_back(tempStr.substr(0, index));
            //Look for more separating characters in the remaining string.
            //index = tempStr.find_first_of(separator);
            tempStr = tempStr.substr(index + 1, tempStr.size() - index);
            index = tempStr.find_first_of(separator);
        }
        else
        {

            //std::cout << tempStr << ": " << index << std::endl;
            if(isInsideDelimeters(tempStr, index, leftDelim, rightDelim))
            {
                //Take the sub string before the separator and store it.
                substrings.push_back(tempStr.substr(0, index));
                tempStr = tempStr.substr(index + 1, tempStr.size() - index);
                index = tempStr.find_first_of(separator);
            }
            else
                index = tempStr.find_first_of(separator, index + 1);
        }
        //std::cout << tempStr << " " <<std::endl;
        //Look for more separating characters in the remaining string.

    }
    //Add the last part of the string
    substrings.push_back(tempStr);

    return substrings;
}

bool FileParser::isInsideDelimeters(const std::string& str, unsigned int charPos, char leftDelim, char rightDelim)
{

    bool inside = true;
    auto temp = charPos;
    while(charPos >= 0 && str[charPos] != leftDelim)
        --charPos;

    if(charPos < 0)
        inside = false;
    else
    {
        charPos = temp;
        while(charPos < str.size() && str[charPos] != rightDelim)
            ++charPos;

        if(charPos >= str.size())
            inside = false;
    }

    return inside;
}

bool FileParser::isValidSectionPath(std::string path)
{
    std::vector<std::string> elements = split(path, '/');
    bool valid = true;

    for(unsigned int i = 0; i < elements.size() && valid; ++i)
        if(trimWhiteSpace(elements[i]) != "" && elements[i][0] != '[' && elements[i].back() != ']')
           valid = false;

    return valid;
}

bool FileParser::isValidAttributePath(std::string path)
{
    std::vector<std::string> elements = split(path, '/');
    bool valid = true;
    std::string sectionPath("");

    //If the last element begins or ends with a square brace, then the path is invalid
    if(elements.back()[0] == '[' || elements.back().back() == ']')
        valid = false;

    //If the last element is an attribute, check the section path
    if(valid)
    {
        sectionPath = getSectionPathFromAttributePath(path);

        valid = isValidSectionPath(sectionPath);

    }

    return valid;

}

std::string FileParser::getSectionPathFromAttributePath(const std::string& attributePath)
{
    std::string sectionPath("");
    std::vector<std::string> elements = split(attributePath, '/');

    for(unsigned int i = 0; i < elements.size() - 1; ++i)
        sectionPath += elements[i] + "/";

    return sectionPath;
}

/** \brief trimWhiteSpace() - Removes all surrounding whitespace around a string.
 *
 * \param str - The original string with surrounding whitespace.
 *
 * \return A new string with all surrounding whitespace removed.
 *
 */
std::string FileParser::trimWhiteSpace(const std::string& str)
{
    std::string trimmedString = str;

    trimmedString.erase(0, trimmedString.find_first_not_of(" \t\n\r\f\v"));
    trimmedString.erase(trimmedString.find_last_not_of(" \t\n\r\f\v") + 1);
    return trimmedString;
}

std::string FileParser::removeOuterSymbols(const std::string& str, char leftSymbol, char rightSymbol)
{
    std::string newString(str);
   // std::cout << "Before String " << str << "\nLeft Symbol " << leftSymbol << "\nRight Symbol " << rightSymbol << "\n";
    if(str.size() >= 1)
    {
        newString = trimWhiteSpace(str);
        unsigned int leftIndex = 0;
        unsigned int rightIndex = newString.size() - 1;

        if(newString[leftIndex] == leftSymbol)
            ++leftIndex;
        if(newString[rightIndex] == rightSymbol)
            --rightIndex;

        if(str.size() > 2 && leftIndex != std::string::npos)
            newString = newString.substr((leftIndex), rightIndex - (leftIndex- 1));
    }
   // std::cout << "After String " << newString << "\n";
    return newString;
}

bool FileParser::searchStringForEndBrace(std::string str)
{
    bool found = false;
    std::size_t endBracePos = str.find_first_of("}");
   // std::cout << str << std::endl;
    while(endBracePos != std::string::npos)
    {
        //std::cout << str << std::endl;
        std::size_t current = endBracePos;

        while(current >= 0 && (str[current] != '\"'))
            --current;

        //If the current character is not a quotation, then there are no quotation marks to the left of the brace
        //so it cannot be surrounded by quotes
        if(str[current] != '\"' || current < 0)
            found = true;
        else
        {

            current = endBracePos;
            while(current < str.size() && (str[current] != '\"'&& str[current] != ','))
                ++current;

            if(current >= str.size())
            {
                found = true;
               //std::cout << str << std::endl;
            }

        }
        if(endBracePos < str.size())
        {
            str = str.substr(endBracePos + 1);
            endBracePos = str.find_first_of("}");
            //std::cout << str << std::endl;
        }
    }

    return found;
}

Attribute FileParser::parseAttributePath(const std::string& attributePath, bool isArray)
{
    Attribute attribute;
    attribute.lineInFile = -1;
    attribute.lastLine = -1;
    std::ifstream in(filePath);

    if(in.is_open())
    {

        std::vector<std::string> pathElements = split(attributePath, '/');

        //Only parse the path if an attribute was provided
        if(pathElements.back()[0] != '[')
        {

            //A section was provided in the path
            if(pathElements.front()[0] == '[')
            {
                std::vector<std::string> sectionNames;
                for(unsigned int i = 0; i < pathElements.size() - 1; ++i)
                    sectionNames.push_back(pathElements[i]);

                std::string sectionPath("");
                for(unsigned int i = 0; i < sectionNames.size(); ++i)
                {
                    if(i < sectionNames.size() - 1)
                        sectionPath += sectionNames[i] + "/";
                    else
                        sectionPath += sectionNames[i];
                }

                //Move to the section that contains the attribute
                int sectionLineNumber = moveToSection(in, sectionPath);

                //Only search the section if it exists
                if(sectionLineNumber != -1)
                    //Search the section for the attribute name
                    attribute = searchSection(in, removeOuterSymbols(sectionNames.back(), '[', ']'), pathElements.back(), isArray);

                //If the attribute was found, add the lines that were consumed to move to the section
                if(attribute.lineInFile != -1)
                {
                    attribute.lineInFile += sectionLineNumber;
                    attribute.lastLine += sectionLineNumber;
                    //std::cout << "First line " << attribute.lineInFile << "\nLast line " << attribute.lastLine << "\n";
                }

            }
            //The attribute is in the default section
            else
            {
                attribute = searchDefaultSection(in, pathElements.front(), isArray);
            }
        }

        in.close();
    }
    else
    {
        std::cerr << "Error opening file " << filePath << std::endl;
    }
    return attribute;
}

/** \brief searchSection - Searches a section for the desired attribute.
 *
 * \param in - the ifstream object that has the open file
 * \param sectionName - The name of the section to search through
 * \param attributeName - The name of the attribute to search for
 * \return If the attribute is found, return an attribute with the line number in the attribute.
 *          If it is not found, the line number will be -1
 *
 */
Attribute FileParser::searchSection(std::ifstream& in, const std::string& sectionName, const std::string& attributeName, bool isArray)
{
    std::string endTag = "[/" + sectionName + "]";
    std::string currentLine;
    Attribute attribute;
    int lineNumber = 0;
    bool attributeFound = false;
    if(in.is_open())
    {
        do
        {
            std::getline(in, currentLine);
            ++lineNumber;
            currentLine = trimWhiteSpace(currentLine);

            if(currentLine.size() > 0 && currentLine[0] != '#')
            {
                auto colonPos = currentLine.find_first_of(':');
                //If the current line contains an attribute
                if(colonPos != std::string::npos)
                {
                    //Split the line up into name and value
                    attribute.name = currentLine.substr(0, colonPos);
                    attribute.name = trimWhiteSpace(attribute.name);
                    attribute.name = removeOuterSymbols(attribute.name, '"', '"');

                    if(attribute.name == attributeName)
                    {
                        attributeFound = true;
                        attribute.lineInFile = lineNumber;
                        attribute.lastLine = attribute.lineInFile;
                        attribute.value = currentLine.substr(colonPos + 1, currentLine.size() - colonPos);

                        if(!isArray)
                        {
                            attribute.value = removeOuterSymbols(trimWhiteSpace(attribute.value), '"', '"');
                        }
                        else
                        {
                            attribute.value = trimWhiteSpace(attribute.value);
                            bool endBraceFound = searchStringForEndBrace(attribute.value);

                            while(!endBraceFound && currentLine != endTag && in.good())
                            {
                                //std::cout << endBraceFound << std::endl;
                                std::getline(in, currentLine);
                                ++attribute.lastLine;
                                currentLine = trimWhiteSpace(currentLine);

                                if(currentLine != endTag)
                                    attribute.value += currentLine;

                                endBraceFound = searchStringForEndBrace(currentLine);
                            }

                            if(!endBraceFound)
                                std::cerr << "Error: End of section reached before finding end of array.\n";

                            //std::cout << attribute.value << std::endl;
                        }
                    }
                }
                //If the current line has the beginning of a subsection
                else if(currentLine[0] == '[' && currentLine[1] != '/')
                {
                    int lines = skipSection(in, removeOuterSymbols(currentLine, '[', ']'));
                    if(lines != -1)
                        lineNumber += lines;
                }
            }

        }while(in.good() && currentLine != endTag && !attributeFound);

    }
    else
    {
        std::cerr << "Error opening " << filePath << std::endl;
    }

    if(!attributeFound)
        attribute.lineInFile = -1;

    return attribute;

}

Attribute FileParser::searchDefaultSection(std::ifstream& in, const std::string& attributeName, bool isArray)
{
    std::cout << "Searching default section\n";
    std::string currentLine;
    Attribute attribute;
    int lineNumber = 0;
    bool attributeFound = false;
    if(in.is_open())
    {
        do
        {
            std::getline(in, currentLine);
            ++lineNumber;
            currentLine = trimWhiteSpace(currentLine);

            //If the current line contains an attribute
            if(currentLine.find(':') != std::string::npos)
            {
                //Split the line up into name and value
                std::vector<std::string> elements = split(currentLine, ':');
                attribute.name = removeOuterSymbols(elements.front(), '"', '"');

                if(attribute.name == attributeName)
                {
                    attributeFound = true;
                    attribute.lineInFile = lineNumber;
                    attribute.lastLine = attribute.lineInFile;

                    if(!isArray)
                    {
                        attribute.value = removeOuterSymbols(trimWhiteSpace(elements.back()), '"', '"');
                    }
                    else
                    {
                        bool endBraceFound = searchStringForEndBrace(elements.back());

                        attribute.value = trimWhiteSpace(elements.back());

                        while(!endBraceFound && in.good())
                        {
                            std::getline(in, currentLine);
                            ++attribute.lastLine;
                            currentLine = trimWhiteSpace(currentLine);

                            attribute.value += currentLine;

                            endBraceFound = searchStringForEndBrace(currentLine);
                        }

                        if(!endBraceFound)
                            std::cerr << "Error: End of section reached before finding end of array.\n";

                        //std::cout << attribute.value << std::endl;
                    }
                }
            }
            //If the current line has the beginning of a subsection
            else if(currentLine[0] == '[' && currentLine[1] != '/')
            {
                int lines = skipSection(in, removeOuterSymbols(currentLine, '[', ']'));
                if(lines != -1)
                    lineNumber += lines;
            }

        }while(in.good() && !attributeFound);
    }
    else
    {
        std::cerr << "Error opening " << filePath << std::endl;
    }

    if(!attributeFound)
        attribute.lineInFile = -1;

    return attribute;
}

int FileParser::moveToSection(std::ifstream& in, const std::string& sectionPath)
{
    std::vector<std::string> sectionNames = split(sectionPath, '/');
    bool lastSectionFound = false;
    unsigned int currentElement = 0;
    std::string currentLine;
    std::string endTag;
    std::string currentSectionName;
    unsigned int lineNumber = -1;
    if(sectionNames.size() >= 1)
    {
        do
        {
            bool currentSectionFound = false;
            bool endOfParentSection = false;
            std::string parentSectionEndTag("");

            //If the current element is a sub-section, set up the parent end tag
            if(sectionNames.size() > 1 && currentElement > 0 && sectionNames[currentElement - 1].size() > 1)
            {
                parentSectionEndTag = sectionNames[currentElement - 1];
                parentSectionEndTag.insert(1, "/");
            }

            //Setup the current section name and end tag
            currentSectionName = removeOuterSymbols(sectionNames[currentElement], '[', ']');
            endTag = "[/" + currentSectionName + "]";

            //Move to the current section until the end of the file is reached, the current section is found
            //or the end of the parent section is reached
            do
            {
                std::getline(in, currentLine);
                ++lineNumber;
                currentLine = trimWhiteSpace(currentLine);
                if(currentLine == "[" + currentSectionName + "]")
                {
                    ++currentElement;
                    currentSectionFound = true;
                  //  std::cout << "Current element " << currentElement << "\nTotal elements " << sectionNames.size() << std::endl;
                }

                else if(parentSectionEndTag != "" && currentLine == parentSectionEndTag)
                {
                    endOfParentSection = true;
                }

            }while(in.good() && !currentSectionFound && !endOfParentSection);



            if(currentSectionFound && currentElement == sectionNames.size())
                lastSectionFound = true;

        }while(in.good() && !lastSectionFound && currentElement < sectionNames.size());
    }
    else
    {
        std::cerr << "No section path provided in function 'moveToSection'\n";
    }

    if(lastSectionFound)
        return lineNumber;
    else
        return -1;
}

int FileParser::skipSection(std::ifstream& in, const std::string& sectionName)
{

    std::string endTag = sectionName;
    bool endTagFound = false;
    std::string currentLine;
    int lineNumber = 1;
    endTag = "[/" + removeOuterSymbols(sectionName, '[', ']') + "]";

    do
    {
        std::getline(in, currentLine);
        currentLine = trimWhiteSpace(currentLine);
        ++lineNumber;

        if(currentLine == endTag)
            endTagFound = true;

    }while(in.good() && !endTagFound);

    if(endTagFound)
        return lineNumber;
    else
    {
        std::cerr << "Found a sub-section with no end tags in " << sectionName << std::endl;
        return -1;
    }

}
