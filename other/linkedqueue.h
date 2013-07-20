// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef LINKEDQUEUE_H
#define LINKEDQUEUE_H

#include <atomic>
#include <SFML/System.hpp>

// Used by LinkedQueue for storing T objects and a pointer to the next node
template <class T>
class Node
{
    public:
        Node(): next(nullptr) {}
        Node(const T& val): next(nullptr), value(val) {}
    //private: // Make these private later
        Node<T>* next;
        T value;
        sf::Mutex inUse;
};

/*
TODO:
    Improve performance by preallocating large groups of nodes at a time and "cycling" through them.
        When there is no room left, allocate another group.
        When using a group less than the total, deallocate a free group.
*/

/*
This class is used for creating queue like objects, implemented as a linked list.
This class is also fully thread safe, and is optimized for use with threads.
    Multiple threads can access the front and back at the same time.
    The locks that are used do not lock the entire list, only the front/back.

Warning: Accessing the front() while pop_front() is called will cause the pointer to
    be dereferenced and accessed while it is being deallocated, which will cause problems!
    This could be fixed by using a unique_ptr and transferring ownership,
        or possibly a shared_ptr so that it acts in a similar way as it does now.
    It would be best to return the unique_ptr with pop_front() and get rid of front().
Warning: front() and back() will both fail if the container is empty.
    Could be fixed with exceptions but we need this to be as fast as possible.
    It should be good enough for the outside code to check if the container is empty
        before trying to access it.
    pop_front() will just skip doing anything if it is empty.
*/

template <class T>
class LinkedQueue
{
    public:
        LinkedQueue(): theSize(0) {}
        ~LinkedQueue() { clear(); }

        typedef Node<T>* NodeTypePtr;

        unsigned int size() { return theSize; }

        bool empty() { return (theSize == 0); }

        T& front()
        {
            sf::Lock firstLock(firstInUse);
            return first->value;
        }

        T& back()
        {
            sf::Lock lastLock(lastInUse);
            return last->value;
        }

        void clear()
        {
            sf::Lock firstLock(firstInUse); // Lock the first
            sf::Lock lastLock(lastInUse); // Lock the last
            if (theSize >= 1) // If there is at least one element then delete the first one
                delete first;
            if (theSize > 1) // If there are 2 or more then delete the last
                delete last;
            // Reset the pointers and size
            first = nullptr;
            last = nullptr;
            theSize = 0;
        }

        void push_back(const T& obj)
        {
            sf::Lock lastLock(lastInUse); // Lock the last
            if (theSize == 0) // The queue is empty
            {
                sf::Lock firstLock(firstInUse);
                first = new Node<T>(obj);
                last = first;
            }
            else // The queue has more than 0 nodes
            {
                last->next = new Node<T>(obj); // Allocate a new node, constructing it with the T type data, and store its pointer as the next pointer of the last node
                last = last->next; // Update the last pointer to the new last node
            }
            theSize++;
        }

        void pop_front()
        {
            if (theSize == 1) // The queue only has 1 node
            {
                sf::Lock firstLock(firstInUse); // Lock the first
                sf::Lock lastLock(lastInUse); // Lock the last
                delete first; // Only delete the 1 node once
                first = nullptr;
                last = nullptr;
                theSize = 0;
            }
            else if (theSize > 1) // The queue has more than 1 node
            {
                sf::Lock firstLock(firstInUse); // Lock the first
                theSize--;
                NodeTypePtr newFirst = first->next; // Temporarily store the 2nd node pointer
                delete first; // Delete the first node
                first = newFirst; // That temporary pointer is our new first node pointer
            }
        }

    private:
        NodeTypePtr first;
        NodeTypePtr last;
        //std::atomic<NodeTypePtr> first;
        //std::atomic<NodeTypePtr> last;
        sf::Mutex firstInUse;
        sf::Mutex lastInUse;
        std::atomic_uint theSize;
};

#endif
