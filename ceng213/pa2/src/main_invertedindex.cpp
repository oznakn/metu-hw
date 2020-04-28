#include <iostream>

#include "InvertedIndex.hpp"

int main() {

    // Testing IIData and InvertedIndex together ...
    std::cout << "Testing IIData and InvertedIndex together ..." << std::endl;

    InvertedIndex ii;

    ii.addDocument("document_1.txt");
    ii.addDocument("document_2.txt");

    ii.printInvertedIndex();

    ii.removeDocument("document_1.txt");

    ii.printInvertedIndex();

    return 0;
}
