#ifndef BSTNODE_HPP
#define BSTNODE_HPP

#include <iostream>

// DO NOT CHANGE THIS FILE.

template <class T>
class BSTNode {
public:
    T data;
    BSTNode<T> *left;
    BSTNode<T> *right;

    BSTNode();
    BSTNode(T data, BSTNode<T> *left, BSTNode<T> *right);

    template <class U>
    friend std::ostream &operator<<(std::ostream &os, const BSTNode<U> &obj);
};

template <class T>
BSTNode<T>::BSTNode() {

    this->left = NULL;
    this->right = NULL;
}

template <class T>
BSTNode<T>::BSTNode(T data, BSTNode<T> *left, BSTNode<T> *right) {

    this->data = data;
    this->left = left;
    this->right = right;
}

template <class T>
std::ostream &operator<<(std::ostream &os, const BSTNode<T> &obj) {

    os << obj.data;

    return os;
}

#endif //BSTNODE_HPP
