#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <iostream>

#include "Node.hpp"

template <class T>
class LinkedList
{
    public: // DO NOT CHANGE THIS PART.
        LinkedList();
        LinkedList(const LinkedList &obj);

        ~LinkedList();

        Node<T> *getFirstNode() const; //
        Node<T> *getHead() const;
        Node<T> *getTail() const;
        int getNumberOfNodes();
        bool isEmpty(); //

        void insertAtTheFront(const T &data); //
        void insertAtTheEnd(const T &data); //
        void insertAfterGivenNode(const T &data, Node<T> *prev); //

        void removeNode(Node<T> *node);
        void removeAllNodes();

        Node<T> *findNode(const T &data); //

        void printAllNodes();
        void printReversed();

        LinkedList &operator=(const LinkedList &rhs);

    private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.

    private: // DO NOT CHANGE THIS PART.
        Node<T> *head;
        Node<T> *tail;
};

template<class T>
LinkedList<T>::LinkedList()
{
    this->head = new Node<T>();
    this->tail = new Node<T>();

    this->head->next = this->tail;
    this->tail->prev = this->head;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList &obj)
{
    this->head = new Node<T>();
    this->tail = new Node<T>();

    this->head->next = this->tail;
    this->tail->prev = this->head;

    Node<T> *start = obj.head->next;

    while (start != NULL && start != obj.tail) {
        this->insertAtTheEnd(start->element);

        start = start->next;
    }
}

template<class T>
LinkedList<T>::~LinkedList()
{
    Node<T> *start = this->head;

    while (start != NULL) {
        Node<T> *next = start->next;

        delete start;

        start = next;
    }
}

template<class T>
Node<T> * LinkedList<T>::getFirstNode() const
{
    if (this->head->next == this->tail) return NULL;

    return this->head->next;
}

template<class T>
Node<T> * LinkedList<T>::getHead() const
{
    return this->head;
}

template<class T>
Node<T> * LinkedList<T>::getTail() const
{
    return this->tail;
}

template<class T>
int LinkedList<T>::getNumberOfNodes()
{
    int number = 0;

    Node<T> *start = this->head->next;

    while (start != NULL && start != this->tail) {
        number++;

        start = start->next;
    }

    return number;
}

template<class T>
bool LinkedList<T>::isEmpty()
{
    return this->getNumberOfNodes() == 0;
}

template<class T>
void LinkedList<T>::insertAtTheFront(const T &data)
{
    Node<T> *node = new Node<T>(data, this->head, this->head->next);

    this->head->next->prev = node;
    this->head->next = node;
}

template<class T>
void LinkedList<T>::insertAtTheEnd(const T &data)
{
    Node<T> *node = new Node<T>(data, this->tail->prev, this->tail);

    this->tail->prev->next = node;
    this->tail->prev = node;
}

template<class T>
void LinkedList<T>::insertAfterGivenNode(const T &data, Node<T> *prev)
{
    Node<T> *node = new Node<T>(data, prev, prev->next);

    prev->next->prev = node;
    prev->next = node;
}

template<class T>
void LinkedList<T>::removeNode(Node<T> *node)
{
    if (node != NULL) {
        bool is_in_list = false;

        Node<T> *start = this->head->next;

        while (start != NULL && start != this->tail) {
            if (node == start) {
                is_in_list = true;
                break;
            }

            start = start->next;
        }

        if (is_in_list) {
            if (node->prev != NULL) {
                node->prev->next = node->next;
            }

            if (node->next != NULL) {
                node->next->prev = node->prev;
            }

            delete node;
        }
    }
}

template<class T>
void LinkedList<T>::removeAllNodes()
{
    Node<T> *start = this->head->next;

    while (start != NULL && start != this->tail) {
        Node<T> *next = start->next;

        delete start;

        start = next;
    }

    this->head->next = this->tail;
    this->tail->prev = this->head;
}

template<class T>
Node<T> * LinkedList<T>::findNode(const T &data)
{
    Node<T> *start = this->head->next;

    while (start != NULL && start != this->tail) {
        if (start->element == data) {
            return start;
        }

        start = start->next;
    }

    return NULL;
}

template<class T>
void LinkedList<T>::printAllNodes()
{
    Node<T> *start = this->head->next;

    while (start != NULL && start != this->tail) {
        std::cout << start->element << std::endl;

        start = start->next;
    }
}

template<class T>
void LinkedList<T>::printReversed()
{
    Node<T> *start = this->tail->prev;

    while (start != NULL && start != this->head) {
        std::cout << start->element << std::endl;

        start = start->prev;
    }
}

template<class T>
LinkedList<T> & LinkedList<T>::operator=(const LinkedList &rhs)
{
    this->removeAllNodes();

    Node<T> *start = rhs.head->next;

    while (start != NULL && start != rhs.tail) {
        this->insertAtTheEnd(start->element);

        start = start->next;
    }

    return *this;
}

#endif //LINKED_LIST_HPP
