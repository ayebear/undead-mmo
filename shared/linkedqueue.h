#ifndef LINKEDQUEUE_H
#define LINKEDQUEUE_H

#include <atomic>

template <class T>
class Node
{
    public:
        Node(): next(nullptr) {}
        Node(T& val): next(nullptr), value(val) {}
    //private: // Make these private later
        Node<T>* next;
        T value;
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
        LinkedQueue();
        T& front();
        T& back();
        void push_back(T&);
        void pop_front(T&);
    private:
        Node<T>* first;
        Node<T>* last;
        std::atomic_bool empty;
        std::atomic_uint size;
};

#endif
