#include <iostream>

#include "BST.hpp"

int main() {

    // Testing BSTNode ...
    std::cout << "Testing BSTNode ..." << std::endl;

    BSTNode<int> node(11, NULL, NULL);

    std::cout << "node : " << node << std::endl;

    // Testing BST ...
    std::cout << "Testing BST ..." << std::endl;

    BST<int> tree;

    tree.insert(10);
    tree.insert(15);
    tree.insert(5);
    tree.insert(13);

    tree.printInorder();

    return 0;
}
