#include "HardCopy.h"
/* YOU MUST WRITE THE IMPLEMENTATIONS OF THE REQUESTED FUNCTIONS
IN THIS FILE. DO NOT MODIFY THE HEADER FILE AS IT WILL BE
REPLACED BY THE ORIGINAL HEADER DURING TESTING. THE REQUIRED
HEADER FILE IS ALREADY INCLUDED FOR YOU ABOVE THIS COMMENT BLOCK.
THE CONSTRUCTOR, DESTRUCTOR and COPY CONSTRUCTOR ARE ALREADY IMPLEMENTED.
START YOUR IMPLEMENTATIONS BELOW THOSE. */


// DONE
HardCopy::HardCopy(int ISBN, double price) : Book(ISBN, price) {

}

// DONE
HardCopy::HardCopy(const HardCopy& hardcopy) {
	this->ISBN = hardcopy.ISBN;
	this->price = hardcopy.price;
	this->next = hardcopy.next;
}

// DONE
HardCopy::~HardCopy() {

}

Book* HardCopy::Clone() const {
    return new HardCopy(this->ISBN, this->price);
}

void HardCopy::Discount() {
    this->price *= 0.9;
}