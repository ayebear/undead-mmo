// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef PACKEDARRAY_H
#define PACKEDARRAY_H

#include "index.h"

/*
This class manages an array of objects in an efficient way, with good cache efficiency and minimal memory allocations.
Adding, removing, and accessing objects are all O(1) operations.

          0   1   2   3   4   (this is how everything outside can access/erase elements)
Outer  : [ ] [2] [ ] [0] [1]  (array of ints, can have "holes" which are actually -1)
Reverse: [3] [4] [1]          (array of ints, parallel to the inner array)
Inner  : [x] [y] [z]          (array of the templated type, no "holes", this should be iterated through)

After erase(3):
Outer  : [ ] [0] [ ] [ ] [1]
Reverse: [1] [4]
Inner  : [z] [y]

Accessing packedArray[1] would return z, packedArray[3] would return x, packedArray[4] would return y.

Example usage:
PackedArray<int> pArr;
int id = pArr.allocate(5);
cout << pArr[id] << endl; // 5
int id2 = pArr.allocate(20);
cout << pArr[id2] << endl; // 20
pArr.erase(id); // Removes 5 from the array
cout << pArr[id2] << endl; // Still 20, even though 20 has moved in memory
*/

template <typename Type>
int swapErase(std::vector<Type>& vec, size_t pos);

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

        // The following functions access everything using the external index IDs

        // Allocates space for a new object and returns its ID
        int allocate()
        {
            int internalId = elements.size();
            elements.resize(internalId + 1);
            return index.insert(internalId);
        }

        // Constructs a new object and returns its ID
        int allocate(const Type& obj)
        {
            int internalId = elements.size();
            elements.push_back(obj);
            return index.insert(internalId);
        }

        // Returns a reference to the object with the specfied ID
        Type& operator[] (int id)
        {
            return elements[index[id]];
        }

        // Returns true if the ID is in range and has an allocated space in the internal array
        bool validId(int id) const
        {
            return (id >= 0 && id < (int)index.size() && index[id] != -1);
        }

        // "Deallocates" an object by swapping and updating the index, so nothing appears to change
        void erase(int id)
        {
            int internalId = index[id]; // Lookup the internal ID of the object
            index.erase(id); // Remove the ID from the index
            int swappedId = swapErase(elements, internalId); // Remove the object
            if (swappedId >= 0) // If another object was swapped
                index.update(reverseLookup[swappedId], internalId); // Update the swapped object's internal ID
            swapErase(reverseLookup, internalId); // Remove the ID from the reverse lookup
            // (Needs to keep the array parallel with the elements array)
        }

        // Returns the current index size
        size_t size() const
        {
            return index.size();
        }

        // The following functions are for accessing the internal array directly

        std::vector<Type>::iterator begin()
        {
            return elements.begin();
        }

        std::vector<Type>::iterator end()
        {
            return elements.end();
        }

        // Returns the current number of allocated elements
        size_t internalSize() const
        {
            return elements.size();
        }

    private:
        std::vector<Type> elements; // Inner array
        std::vector<int> reverseLookup; // This is used for deleting elements and updating the proper IDs in the normal lookup
        Index<int> index; // This is used for accessing elements by their external ID

};

template <typename Type>
int swapErase(std::vector<Type>& vec, size_t pos)
{
    // This function overwrites the specified element with the last element
    // Returns the position of the element that was moved, or -1 if nothing else was affected
    int oldPos = -1;
    if (pos < vec.size() && !vec.empty())
    {
        if (vec.size() == 1)
            vec.pop_back(); // Remove the last and only object
        else
        {
            vec[pos] = vec.back(); // Replace the old object with the last one
            vec.pop_back(); // Remove the last object
            oldPos = vec.size(); // The ID of the old last object
        }
    }
    return oldPos;
}

#endif
