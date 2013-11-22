// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef OBJECTALLOCATOR_H
#define OBJECTALLOCATOR_H

#include <vector>
#include <queue>

using std::size_t;

/*
This class manages a vector of objects in an efficient way to reduce the amount of memory allocations.
TODO:
    Look into a more efficient container than a vector for resizing, we don't want it to copy all of the elements whenever it resizes.
    size_t is being used as the ID type, maybe make this a typedef in the class (will have to use typename or something as well)
    Figure out ways to make this more robust, such as using a parallel bool array (or 2 queues) for determining which objects are allocated/unused.
*/
template <class Type>
class ObjectAllocator
{
    public:
        ObjectAllocator(size_t = 32);
        ~ObjectAllocator();

        //size_t allocate(const Type&); // Will we even need this?
        size_t allocate(); // Returns an ID to an unused object
        bool find(size_t) const; // Returns true if this ID has an object already allocated
        Type& operator[] (size_t); // Returns a reference to the object with the specfied ID
        void erase(size_t); // "Deallocates" an object

    private:
        bool validId(size_t) const;
        void preallocate(size_t);

        size_t chunkSize;
        std::vector<Type> elements;
        std::queue<size_t> freeIds;
};

template <class Type>
ObjectAllocator<Type>::ObjectAllocator(size_t chunks):
    chunkSize(chunks)
{
}

template <class Type>
ObjectAllocator<Type>::~ObjectAllocator()
{
}

template <class Type>
size_t ObjectAllocator<Type>::allocate()
{
    size_t id = -1;
    if (freeIds.empty())
        preallocate(chunkSize);
    id = freeIds.front();
    freeIds.pop();
    /*
    if (freeIds.empty())
    {
        id = elements.size();
        //elements.push_back(value);
        preallocate(chunkSize);
    }
    else
    {
        id = freeIds.front();
        freeIds.pop();
        //elements[id] = value;
    }
    */
    return id;
}

template <class Type>
bool ObjectAllocator<Type>::find(size_t id) const
{
    // Can't properly determine this unless we keep track of the used IDs
    return validId(id);
}

template <class Type>
Type& ObjectAllocator<Type>::operator[] (size_t id)
{
    return elements[id];
}

template <class Type>
void ObjectAllocator<Type>::erase(size_t id)
{
    if (validId(id))
        freeIds.push(id);
}

template <class Type>
bool ObjectAllocator<Type>::validId(size_t id) const
{
    return (id < elements.size());
}

template <class Type>
void ObjectAllocator<Type>::preallocate(size_t chunks)
{
    size_t oldSize = elements.size();
    elements.resize(oldSize + chunks);
    for (size_t i = oldSize; i < elements.size(); ++i)
        freeIds.push(i);
}

#endif
