#ifndef LINKEDQUEUE_H
#define LINKEDQUEUE_H

#include <atomic>
#include <SFML/System.hpp>

template <class T>
class Node
{
    public:
        Node(): next(nullptr) {}
        Node(T& val): next(nullptr), value(val) {}
    //private: // Make these private later
        Node<T>* next;
        T value;
        sf::Mutex inUse;
};

/*
This class is used for creating queue like objects, implemented as a linked list.
This class is also fully thread safe, and is optimized for use with threads.
    Multiple threads can access the front and back at the same time.
    The locks that are used do not lock the entire list, only portions of it.
*/

template <class T>
class LinkedQueue
{
    public:
        LinkedQueue(): theSize(0) {}

        typedef Node<T>* NodeTypePtr;

        bool empty() { return (theSize == 0); }
        T& front() { return *first; }
        T& back() { return *last; }

        void push_back(T& obj)
        {
            sf::Lock lastLock(lastInUse); // Lock the last
            if (theSize == 0) // The queue is empty
            {
                last = new Node<T>(obj);
            }
            else if (theSize == 1) // The queue only has 1 node
            {
                //
            }
            else // The queue has more than 1 node
            {
                sf::Lock lock(last->inUse); // Lock the node itself
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
            }
            else if (theSize > 1)
            {
                sf::Lock firstLock(firstInUse); // Lock the first
                NodeTypePtr newFirst = first->next; // Temporarily store the 2nd node pointer
                delete first; // Delete the first node
                first = newFirst; // That temporary pointer is our new first node pointer
            }
        }

    private:
        std::atomic<NodeTypePtr> first;
        std::atomic<NodeTypePtr> last;
        sf::Mutex firstInUse;
        sf::Mutex lastInUse;
        std::atomic_uint theSize;
};

#endif
