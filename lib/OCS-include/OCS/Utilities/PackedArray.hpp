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

#ifndef PACKEDARRAY_H
#define PACKEDARRAY_H

#include <vector>
#include <stack>
#include <utility>
#include <stdint.h>
#include <initializer_list>
#include <iostream>

typedef uint64_t Index;

/** \brief Polymorphic class for PackedArrays
 *
 */
class BasePackedArray
{
    public:
        virtual Index createCopy(Index) = 0;
        virtual Index remove(Index) = 0;
        virtual void clear() = 0;
        virtual Index size() const = 0;
        virtual bool isValid(Index) const = 0;
};

/** \brief A wrapper around a vector trades space efficiency for a constant time remove function. All other
 *         indexes are unaffected through the use of lookup tables.
 *
 */
template <typename T>
class PackedArray : public BasePackedArray
{
    public:
        PackedArray() {}

        PackedArray (std::initializer_list<T> iList)
        {
            for(auto it = iList.begin(); it != iList.end(); ++it)
                add_item(*it);
        }

        PackedArray(std::size_t numberToReserve)
        {
            if(numberToReserve > 0)
            {
                elements.reserve(numberToReserve);
                elementIndeces.reserve(numberToReserve);
                reverseLookupList.reserve(numberToReserve);
            }
        }

        typename std::vector<T>::iterator begin()
        {
            return elements.begin();
        }

        typename std::vector<T>::const_iterator begin() const
        {
            return elements.cbegin();
        }

        typename std::vector<T>::iterator end()
        {
            return elements.end();
        }

        typename std::vector<T>::const_iterator end() const
        {
            return elements.cend();
        }

        T& operator[](Index idx)
        {
            return elements[ elementIndeces[idx] ];
        }

        const T& operator[](Index idx) const
        {
            return elements[ elementIndeces[idx] ];
        }

        Index size() const
        {
            return elements.size();
        }

        Index add_item(const T& item)
        {
            Index newIdx;
            elements.push_back(item);

            if(availableIndeces.size() > 0)
            {
                newIdx = availableIndeces.top();
                elementIndeces[newIdx] = size() - 1;
                reverseLookupList[size() - 1] = newIdx;
                availableIndeces.pop();
            }
            else
            {
                newIdx = elements.size() - 1;
                elementIndeces.push_back(newIdx);
                reverseLookupList.push_back(newIdx);
            }
            return newIdx;
        }

        template<typename ... Args>
        Index emplace_item(Args&& ... args)
        {
            T newItem(std::forward<Args>(args) ... );
            return add_item(newItem);
        }

        Index createCopy(Index indexToCopy)
        {
            if(indexToCopy < elements.size())
                return add_item(elements[indexToCopy]);
            return -1;
        }

        bool isValid(Index idx) const
        {
            if(idx >= elementIndeces.size() || elementIndeces[idx] == -1)
                return false;

            return true;
        }

        Index remove(Index idx)
        {
            Index swappedIndex = -1;

            if(idx < elementIndeces.size())
            {

                Index indexToRemove = elementIndeces[idx];
                swappedIndex = reverseLookupList[size() - 1];

                elementIndeces[swappedIndex] = indexToRemove;
                reverseLookupList[indexToRemove] = swappedIndex;

                elements[indexToRemove] = elements[size() - 1];
                elements.pop_back();

                elementIndeces[indexToRemove] = -1;

                availableIndeces.push(idx);
            }

            return swappedIndex;
        }

        void clear()
        {
            elements.clear();
            reverseLookupList.clear();
            elementIndeces.clear();

            while(availableIndeces.size() > 0)
                availableIndeces.pop();
        }

    private:

        std::vector<T> elements;
        std::vector<Index> elementIndeces;
        std::vector<Index> reverseLookupList;
        std::stack<Index> availableIndeces;

};

template<typename T>
std::ostream& operator<<(std::ostream& out, const PackedArray<T>& arry)
{
    for(auto& i : arry)
        out << i << " ";
    return out;
}

#endif // PACKEDARRAY_H
