// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef OBJECTALLOCATOR_H
#define OBJECTALLOCATOR_H

#include <vector>

/// This class manages a vector of objects in an efficient way to reduce the amount of memory allocations.
template <class Type>
class ObjectAllocator
{
    public:

        ObjectAllocator()
        {
        }

        // Pre-allocate an amount of elements
        ObjectAllocator(unsigned elementsToReserve)
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

        // Returns true if this ID has an object already allocated
        bool find(int id) const
        {
            return (id < elements.size());
        }

        // Returns a reference to the object with the specfied ID
        Type& operator[] (int id)
        {
            return elements[id];
        }

        // "Deallocates" an object by swapping, returns the ID of the swapped element
        int erase(int id)
        {
            // This function will replace the erased object with the last one in the vector.
            // Therefore it is important to update the ID of the object that was moved (the ID is returned).
            int oldId = -1; // Returns -1 if nothing is erased or if no action is required
            if (find(id))
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
            return elements.size();
        }

    private:

        std::vector<Type> elements;

};

#endif
