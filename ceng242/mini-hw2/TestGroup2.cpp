#include "Book.h"
#include "SoftCopy.h"

// TestGroup 2 : ""SoftCopy UploadPage() Tests""

// COMPILE: g++ -std=c++11 TestGroup2.cpp Book.cpp  SoftCopy.cpp -o test
// RUN: valgrind --leak-check=full ./test

int main() {

    /************************ Construct the books ************************/

    // Create SoftCopies
    SoftCopy* s1 = new SoftCopy(240, 12.0, 0, 18);
    SoftCopy* s2 = new SoftCopy(240, 8.6, 10, 14);
    SoftCopy* s3 = new SoftCopy(240, 17.0, 4, 6);

    SoftCopy* r1 = new SoftCopy(220, 15.0, 0, 7);
    SoftCopy* w1 = new SoftCopy(250, 45.8, 0, 1025);    // Large book

    /********************* Prepare softcopy page content ******************/
    string s_pages[20];
    for (int i=0; i<20; i++) {
        string part1 = "PAGE CONTENT ";
        string part2 = ", in SOFT COPY 240";
        s_pages[i] = part1 + to_string(i) + part2;
    }

    string r_pages[8];
    for (int i=0; i<8; i++) {
        string part1 = "PAGE CONTENT ";
        string part2 = ", in SOFT COPY 220";
        r_pages[i] = part1 + to_string(i) + part2;
    }

    string w_pages[8];
    int w_page_ids[8] = {825, 417, 100, 9, 310, 781, 615, 986};
    for (int i=0; i<8; i++) {
        string part1 = "PAGE CONTENT ";
        string part2 = ", in SOFT COPY 250";
        w_pages[i] = part1 + to_string(w_page_ids[i]) + part2;
    }

    /************************** UploadPage() Tests ***********************/
    // Upload all pages of r1, build full tree
    for (int i=0; i<8; i++)
        r1->UploadPage(r_pages[i], i);

    // Upload specific pages of s1
    s1->UploadPage(s_pages[0], 0);
    s1->UploadPage(s_pages[3], 3);
    s1->UploadPage(s_pages[17], 17);

    // Upload specific pages of s2
    s2->UploadPage(s_pages[10], 10);
    s2->UploadPage(s_pages[11], 11);
    s2->UploadPage(s_pages[12], 12);
    s2->UploadPage(s_pages[13], 13);
    s2->UploadPage(s_pages[14], 14);

    // Upload specific pages of s3
    s3->UploadPage(s_pages[4], 4);
    s3->UploadPage(s_pages[5], 5);
    s3->UploadPage(s_pages[6], 6);

    // Upload specific pages of w1
    for (int i=0; i<8; i++)
        w1->UploadPage(w_pages[i], w_page_ids[i]);

    /************************ Delete the softcopies ***********************/
    delete s1;  s1 = NULL;
    delete s2;  s2 = NULL;
    delete s3;  s3 = NULL;
    delete r1;  r1 = NULL;
    delete w1;  w1 = NULL;

    return 0;

}
