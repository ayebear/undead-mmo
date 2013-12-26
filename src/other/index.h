#ifndef INDEX_H
#define INDEX_H

#include <vector>
#include <deque>

/*
This class contains an index for mapping external IDs to internal IDs.
It is templated in case you want to use certain sized integers.
*/
template <class Type>
class Index
{
    public:

        Index()
        {
        }

        // Returns an internal ID
        Type operator[](Type id) const
        {
            return idList[id];
        }

        // Returns size of lookup table (including holes)
        size_t size() const
        {
            return idList.size();
        }

        // Returns size of lookup table (excluding holes)
        size_t virtualSize() const
        {
            return (idList.size() - freeList.size());
        }

        // Sets an existing ID to a new ID
        void update(Type id, Type newId)
        {
            idList[id] = newId;
        }

        // Adds an internal ID to the index, returning the new external ID
        Type insert(Type internalId)
        {
            Type newId;
            if (freeList.empty())
            {
                newId = idList.size();
                idList.push_back(internalId);
            }
            else
            {
                newId = freeList.front();
                freeList.pop_front();
                idList[newId] = internalId;
            }
            return newId;
        }

        // Removes an external ID from the index
        void erase(Type externalId)
        {
            freeList.push_back(idList[externalId]);
            idList[externalId] = -1;
        }

        // Removes all IDs
        void clear()
        {
            idList.clear();
            freeList.clear();
        }

    private:

        std::vector<Type> idList;
        std::deque<Type> freeList;

        // TODO: Use a fake linked list type thing inside of the idList with IDs stored for the
        // next free node, so that there won't need to be a separate queue for the free list.
        // Also benchmark this to see if it is worth using instead of the separate deque.
};

#endif
