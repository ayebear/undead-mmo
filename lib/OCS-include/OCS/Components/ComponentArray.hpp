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

#ifndef COMPONENTARRAY_H
#define COMPONENTARRAY_H

#include <iostream>

#include <OCS/Components/Component.hpp>
#include <OCS/Utilities/PackedArray.hpp>

struct BaseComponentArray
{
    virtual ~BaseComponentArray() {}

    virtual ocs::BaseComponent& getBaseComponent(Index) = 0;
    virtual Index createCopy(Index) = 0;
    virtual Index createCopy(Index, BaseComponentArray*) = 0;
    virtual Index remove(Index) = 0;
    virtual void clear() = 0;
    virtual Index size() const = 0;

    virtual Index add_item(const std::string&) = 0;

};

template<typename C>
struct ComponentArray : public BaseComponentArray
{
    public:

        ocs::BaseComponent& getBaseComponent(Index idx) { return arry[idx]; }

        C& operator[](Index idx) { return arry[idx]; }
        const C& operator[](Index idx) const { return arry[idx]; }

        typename std::vector<C>::iterator begin() { return arry.begin(); }
        typename std::vector<C>::const_iterator begin() const { return arry.begin(); }
        typename std::vector<C>::iterator end() { return arry.end(); }
        typename std::vector<C>::const_iterator end() const { return arry.cend(); }

        template<typename ... Args>
        Index emplace_item(Args&& ... args) { return arry.emplace_item(args...); }
        Index add_item(const C& item) { return arry.add_item(item); }
        Index createCopy(Index idx) { return arry.createCopy(idx); }

        Index createCopy(Index idx, BaseComponentArray* otherArry)
        {

            auto otherCompArry = static_cast<ComponentArray<C>*>(otherArry);

            if(otherCompArry)
            {
                C newItem = arry[idx];

                return otherCompArry->add_item(newItem);
            }
            return -1;

        }

        Index remove(Index idx) { return arry.remove(idx); }
        void clear() { arry.clear(); }
        Index size() const { return arry.size(); }

        Index add_item(const std::string& compValues)
        {
            C newItem;
            newItem.deSerialize(compValues);

            return arry.add_item(newItem);
        }

    private:

        PackedArray<C> arry;

};

#endif // COMPONENTARRAY_H
