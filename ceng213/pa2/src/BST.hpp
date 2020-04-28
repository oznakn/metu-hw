#ifndef BST_HPP
#define BST_HPP

#include <iostream>

#include "BSTNode.hpp"

template <class T>
class BST {
public: // DO NOT CHANGE THIS PART.
    BST();
    BST(const BST<T> &obj);

    ~BST();

    BSTNode<T> *getRoot();

    void insert(const T &data);
    void remove(const T &data);
    BSTNode<T> *find(const T &data);
    void printInorder();

    BST<T> &operator=(const BST<T> &rhs);

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.
    void printInorder(BSTNode<T> *node);

    T getMinUsingNode(BSTNode<T> * &node);

    void insertUsingNode(BSTNode<T> * &node, const T &data);
    BSTNode<T> *findUsingNode(BSTNode<T> * &node, const T &data);
    void removeUsingNode(BSTNode<T> * &node, const T &data);
    BSTNode<T> *copyUsingNode(const BSTNode<T> *node);
    void deleteUsingNode(BSTNode<T> * &node);

private: // DO NOT CHANGE THIS PART.
    BSTNode<T> *root;
};

#endif //BST_HPP

template <class T>
BST<T>::BST() {
    this->root = NULL;
}

template <class T>
BST<T>::BST(const BST<T> &obj) {
    this->root = this->copyUsingNode(obj.root);
}

template <class T>
BST<T>::~BST() {
    this->deleteUsingNode(this->root);
}

template <class T>
BSTNode<T> *BST<T>::getRoot() {
    return this->root;
}

template <class T>
void BST<T>::insert(const T &data) {
    this->insertUsingNode(this->root, data);
}

template <class T>
void BST<T>::remove(const T &data) {
    this->removeUsingNode(this->root, data);
}

template <class T>
BSTNode<T> *BST<T>::find(const T &data) {
    return this->findUsingNode(this->root, data);
}

template <class T>
void BST<T>::printInorder() {

    if (root == NULL) {
        std::cout << "{}" << std::endl;
    } else {
        std::cout << "{" << std::endl;
        printInorder(root);
        std::cout << std::endl << "}" << std::endl;
    }
}

template <class T>
void BST<T>::printInorder(BSTNode<T> *node) {

    if (node == NULL)
        return;

    printInorder(node->left);
    if (node->left) {
        std::cout << "," << std::endl;
    }
    std::cout << "\t" << node->data;
    if (node->right) {
        std::cout << "," << std::endl;
    }
    printInorder(node->right);
}

template <class T>
BST<T> &BST<T>::operator=(const BST<T> &rhs) {
    this->deleteUsingNode(this->root);

    this->root = this->copyUsingNode(rhs.root);
}

// helper functions written by oznakn
template <class T>
T BST<T>::getMinUsingNode(BSTNode<T> * &node) {
    if (node->left == NULL) {
        return node->data;
    }

    return this->getMinUsingNode(node->left);
}


template <class T>
void BST<T>::insertUsingNode(BSTNode<T> * &node, const T &data) {
    if (node == NULL) {
        node = new BSTNode<T>(data, NULL, NULL);
    }
    else if (data < node->data) {
        this->insertUsingNode(node->left, data);
    }
    else if (data > node->data) {
        this->insertUsingNode(node->right, data);
    }
}

template <class T>
BSTNode<T> *BST<T>::findUsingNode(BSTNode<T> * &node, const T &data) {
    if (node == NULL) {
        return NULL;
    }

    if (node->data == data) {
        return node;
    }

    if (node->data > data) {
        return this->findUsingNode(node->left, data);
    }

    if (node->data < data) {
        return this->findUsingNode(node->right, data);
    }

    return NULL;
}

template <class T>
void BST<T>::removeUsingNode(BSTNode<T> * &node, const T &data) {
    if (node != NULL) {
        if (node->data > data) {
            this->removeUsingNode(node->left, data);
        }
        else if (node->data < data) {
            this->removeUsingNode(node->right, data);
        }
        else {
            if (node->left != NULL && node->right != NULL) {
                node->data = this->getMinUsingNode(node->right);

                this->removeUsingNode(node->right, node->data);
            }
            else {
                BSTNode<T> *old_node = node;

                if (node->left != NULL) {
                    node = node->left;
                }
                else {
                    node = node->right;
                }

                delete old_node;
            }
        }
    }
}

template <class T>
BSTNode<T> *BST<T>::copyUsingNode(const BSTNode<T> *node) {
    if (node != NULL) {
        return new BSTNode<T>(node->data,
                    this->copyUsingNode(node->left),
                    this->copyUsingNode(node->right)
                );
    }

    return NULL;
}


template <class T>
void BST<T>::deleteUsingNode(BSTNode<T> * &node) {
    if (node != NULL) {
        this->deleteUsingNode(node->left);
        this->deleteUsingNode(node->right);
        delete node;
    }
}
