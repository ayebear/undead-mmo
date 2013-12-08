// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef PACKEDARRAY_H
#define PACKEDARRAY_H

#include <vector>

/*
This class manages an array of objects in an efficient way, with good cache efficiency and minimal memory allocations.
Adding, removing, and accessing objects are all O(1) operations.

There are two arrays: the inner one stores the actual objects, the outer one is just for mapping IDs to the inner array.
        0   1   2   3   4   (this is how everything outside can access/erase elements)
Outer: [ ] [2] [ ] [0] [1]  (array of ints, can have "holes" which are actually -1)
Inner: [x] [y] [z]          (array of the templated type, no "holes", this should be iterated through)
*/
template <class Type>
class PackedArray
{
    public:

        PackedArray()
        {
        }

        // Pre-allocate an amount of elements
        PackedArray(unsigned elementsToReserve)
        {
            elements.reserve(elementsToReserve);
        }

        // Allocates space for a new object and returns its ID
        int allocate()
        {
            elements.resize(elements.size() + 1);
            return (elements.size() - 1);
        }

        // Constructs a new object and returns its ID
        int allocate(const Type& obj)
        {
            elements.push_back(obj);
            return (elements.size() - 1);
        }

        // Returns a reference to the object with the specfied ID
        Type& operator[] (int id)
        {
            return elements[index[id]];
        }

        // "Deallocates" an object by swapping, returns the ID of the swapped element
        int erase(int id)
        {
            // This function will replace the erased object with the last one in the vector.
            // Therefore it is important to update the ID of the object that was moved (the ID is returned).
            int oldId = -1; // Returns -1 if nothing is erased or if no action is required
            if (id < elements.size())
            {
                if (elements.size() > 0)
                {
                    elements[id] = elements.back(); // Replace the old object with the last one
                    elements.pop_back(); // Remove the last object
                    oldId = elements.size(); // The ID of the old last object, which now has the ID of the erased element
                }
                else if (elements.size() == 1)
                    elements.pop_back(); // Remove the last and only object
            }
            return oldId;
        }

        // Returns the current size
        int size()
        {
            return index.size();
        }

        std::vector<Type>::iterator begin()
        {
            elements.begin();
        }

        std::vector<Type>::iterator end()
        {
            elements.end();
        }

    private:

        std::vector<Type> elements; // Inner array
        std::vector<int> index; // Outer array
        std::queue<int> freeList; // Available IDs in the outer array

};

#endif
