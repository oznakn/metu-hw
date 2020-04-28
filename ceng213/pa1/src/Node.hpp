#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>

// DO NOT CHANGE THIS FILE.

template<class T>
class Node
{
    public:
        T element;
        Node<T> *prev;
        Node<T> *next;

        Node();
        Node(T element, Node<T> *prev, Node<T> *next);
        Node(const Node<T> &obj);

        friend std::ostream &operator<<(std::ostream &os, const Node<T> &obj)
        {
            os << obj.element;
            return os;
        }
};

template<class T>
Node<T>::Node()
{
    this->prev = NULL;
    this->next = NULL;
}

template<class T>
Node<T>::Node(T element, Node<T> *prev, Node<T> *next)
{
    this->element = element;
    this->prev = prev;
    this->next = next;
}

template<class T>
Node<T>::Node(const Node<T> &obj)
{
    this->element = obj.element;
    this->prev = obj.prev;
    this->next = obj.next;
}

#endif //NODE_HPP
