#include "InvertedIndex.hpp"

#include <fstream>

InvertedIndex::InvertedIndex() {

}

InvertedIndex::InvertedIndex(const InvertedIndex &obj) {

    this->invertedIndex = obj.invertedIndex; // uses operator= of BST
}

InvertedIndex::~InvertedIndex() {

}

BST<IIData> &InvertedIndex::getInvertedIndex() {

    return invertedIndex;
}

InvertedIndex &InvertedIndex::addDocument(const std::string &documentName) {

    std::ifstream file(documentName.c_str(), std::ifstream::in);

    std::string word;
    for (int position = 1; (file >> word); ++position) {
        BSTNode<IIData> *theNode = invertedIndex.find(IIData(word));

        if (theNode == NULL) {
            this->invertedIndex.insert(IIData(word));

            theNode = invertedIndex.find(IIData(word));
        }

        theNode->data.addOccurrence(documentName, position);
    }

    file.close();

    return *this;
}

InvertedIndex &InvertedIndex::removeDocument(const std::string &documentName) {
    BSTNode<IIData> *root = this->invertedIndex.getRoot();

    if (root != NULL) {
        this->removeDocumentUsingNode(root, documentName);
    }

    return *this;
}

InvertedIndex &InvertedIndex::removeDocumentUsingNode(BSTNode<IIData> *node, const std::string &documentName) {
    if (node != NULL) {
        this->removeDocumentUsingNode(node->left, documentName);
        this->removeDocumentUsingNode(node->right, documentName);

        node->data.removeOccurrences(documentName);

        if (node->data.getOccurrences().size() == 0) {
            this->invertedIndex.remove(node->data);
        }
    }
}


InvertedIndex &InvertedIndex::searchWord(const std::string &word) {

    BSTNode<IIData> *theNode = invertedIndex.find(IIData(word));

    if (theNode == NULL) {
        std::cout << "The word \"" << word << "\" does not exist in inverted index." << std::endl;
    } else {
        std::cout << theNode->data << std::endl;
    }

    return *this;
}

InvertedIndex &InvertedIndex::printInvertedIndex() {

    invertedIndex.printInorder();

    return *this;
}

InvertedIndex &InvertedIndex::operator=(const InvertedIndex &rhs) {

    this->invertedIndex = rhs.invertedIndex; // uses operator= of BST

    return *this;
}
