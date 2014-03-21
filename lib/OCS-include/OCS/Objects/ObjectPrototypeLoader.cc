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

#include "OCS/Objects/ObjectPrototypeLoader.hpp"
#include "OCS/Objects/ObjectManager.hpp"
#include "OCS/Test_Files/SampleComponents.hpp"
#include "OCS/Utilities/FileParser.hpp"

namespace ocs
{


/** \brief Load an object prototype from a file.
 *
 * \param filePath The path to the file that contains the object prototype.
 * \param prototypeName The name of the prototype to load
 * \param prototypeParentSection(Optional) The section that the prototype's section is located in.
 *
 * \return true if the prototype was loaded. False if the prototype already exists or the prototype does not have a valid section.
 *
 */
bool ObjectPrototypeLoader::loadObjectPrototype(ObjectManager& objManager, const std::string& filePath, const std::string& prototypeName, const std::string& prototypeParentSection)
{
    //Only load the template if one does not exist under that name
    if(!doesPrototypeExist(objManager, prototypeName))
    {
        //std::cout << "Loading prototype " << prototypeName << std::endl;

        FileParser parser(filePath, false);
        std::vector<std::string> componentTypes;
        std::string prototypeSection = "[" + prototypeName + "]";
        std::string sectionPath("");

        //If the prototype is located in another section, build the section path.
        if(prototypeParentSection != "")
            sectionPath = prototypeParentSection + "/";
        sectionPath += prototypeSection;

        if(!parser.doesSectionExist(sectionPath))
        {
            std::cerr << "Error loading prototype '" << prototypeName << "'. Prototypes section does not exist.\n";
            return false;
        }

        //Get a list of the prototype's components.
        parser.loadArray(sectionPath, "Components", componentTypes);

        //Iterate through all
        for(const auto& compName : componentTypes)
        {
            std::string componentValues = parser.getAttributeAsString(sectionPath, compName);
            objManager.addComponentToPrototypeFromString(prototypeName, compName, componentValues);
        }

       // std::cout << "Finished loading prototype " << prototypeName << "!\n\n";
        return true;
    }

    return false;
}

/** \brief Load multiple object prototypes.
 *
 * \param filePath The path to the file containing the object prototype set.
 * \param setName The name of the attribute that holds the list of prototype names.
 * \param parentSection The name of the outer section containing the prototype set attribute.
 *
 * \return The total number of prototypes that were loaded.
 *
 */
int ObjectPrototypeLoader::loadPrototypeSet(ObjectManager& objManager, const std::string& filePath, const std::string& setName, const std::string& parentSection)
{
    unsigned int totalPrototypesAdded = 0;

    FileParser parser(filePath, false);
    std::vector<std::string> prototypeElements;
    parser.loadArray(parentSection, setName, prototypeElements);

    for(unsigned int i = 0; i < prototypeElements.size(); ++i)
    {
        bool prototypeAdded = loadObjectPrototype(objManager, filePath, prototypeElements[i]);

        if(prototypeAdded)
            ++totalPrototypesAdded;
    }
    //std::cout << "\nAdded " << totalPrototypesAdded << " prototypes\n";
    return totalPrototypesAdded;
}

/** \brief Checks if the given name is already in use by an object prototype.
 *
 * \param prototypeName The prototype name in question.
 *
 * \return True if the prototype name is already in use. False if it is available.
 *
 */
bool ObjectPrototypeLoader::doesPrototypeExist(const ObjectManager& objManager, const std::string& prototypeName)
{
    return objManager.doesPrototypeExist(prototypeName);
}

}//ocs

