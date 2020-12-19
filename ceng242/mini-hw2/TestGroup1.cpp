#include "Book.h"
#include "HardCopy.h"
#include "SoftCopy.h"

// TestGroup 1 : "< and > Operators Overload and Discount() Tests"

// COMPILE: g++ -std=c++11 TestGroup1.cpp Book.cpp HardCopy.cpp SoftCopy.cpp -o test
// RUN: valgrind --leak-check=full ./test

int main() {

    Book* books[8];
    // Create HardCopies and SoftCopies
    books[0] = new HardCopy(120, 30.5);
    books[1] = new SoftCopy(130, 20.2, 0, 100);
    books[2] = new HardCopy(150, 60.75);
    books[3] = new HardCopy(170, 15.1);
    books[4] = new HardCopy(180, 18.3);
    books[5] = new HardCopy(190, 25.5);
    books[6] = new SoftCopy(200, 10.5, 0, 85);
    books[7] = new SoftCopy(160, 50.0, 0, 9);

    /************************ Compare book ISBNs ************************/
    // Operator <
    if (*books[0] < 150);    // 120 < 150 ?
    if (*books[0] < 100);    // 120 < 100 ?

    // Operator >
    if (*books[3] > 160);    // 170 > 160 ?
    if (*books[3] > 180);    // 170 > 180 ?

    // Operator < , comparison with another book
    if (*books[1] < *books[2]);    // 130 < 150 ?
    if (*books[2] < *books[1]);    // 150 < 130 ?

    // Check equality
    if (*books[2] < 150);    // 150 < 150 ?
    if (*books[2] > 150);    // 150 < 150 ?

    HardCopy temp(150, 10.0);
    if (*books[2] < temp);   // 150 < 150 ?

    /************************** Discount() Tests **************************/

    // HardCopy discount
    books[5]->Discount();   // new price: 22.95

    // SoftCopy discount
    books[6]->Discount();   // new price: 7.875

    /************************ Delete the books ************************/
    for (int i=0; i < 8; i++) {
        if (books[i] != NULL) {
            delete books[i];
            books[i] = NULL;
        }
    }

    return 0;

}
