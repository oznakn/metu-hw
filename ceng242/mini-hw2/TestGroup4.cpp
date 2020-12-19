#include "Book.h"
#include "SoftCopy.h"

// TestGroup 4 : "SoftCopy Display() Tests"

// COMPILE: g++ -std=c++11 TestGroup4.cpp Book.cpp  SoftCopy.cpp -o test
// RUN: valgrind --leak-check=full ./test

int main() {

    /************************ Construct the books ************************/

    // Create SoftCopies
    SoftCopy* s1 = new SoftCopy(240, 12.0, 0, 18);      // 19 sayfalık kitap
    SoftCopy* s2 = new SoftCopy(240, 8.6, 10, 14);      // 5 sayfalık parça
    SoftCopy* s3 = new SoftCopy(240, 17.0, 4, 6);       // 3 sayfalık parça

    SoftCopy* r1 = new SoftCopy(220, 15.0, 0, 7);       // pow(2,3)= 8 sayfalık kitap

    SoftCopy* t1 = new SoftCopy(230, 16.5, 0, 15);      // pow(2,4) = 16 sayfalık kitap
    SoftCopy* t2 = new SoftCopy(230, 2, 10, 11);        // 2 sayfalık exact branch

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

    string t_pages[16];
    for (int i=0; i<16; i++) {
        string part1 = "PAGE CONTENT ";
        string part2 = ", in SOFT COPY 230";
        t_pages[i] = part1 + to_string(i) + part2;
    }

    string w_pages[8];
    int w_page_ids[8] = {825, 417, 100, 9, 310, 781, 615, 986};
    for (int i=0; i<8; i++) {
        string part1 = "PAGE CONTENT ";
        string part2 = ", in SOFT COPY 250";
        w_pages[i] = part1 + to_string(w_page_ids[i]) + part2;
    }

    /****************************** Upload Pages ****************************/
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

    // Upload specific pages of t1
    t1->UploadPage(t_pages[1], 1);

    // Upload specific pages of t2
    t2->UploadPage(t_pages[10], 10);
    t2->UploadPage(t_pages[11], 11);

    // Upload specific pages of w1
    for (int i=0; i<8; i++)
        w1->UploadPage(w_pages[i], w_page_ids[i]);

    /****************************** Display Pages ****************************/
    string output = r1->Display(0, 7);
    output = s1->Display(0, 5);
    output = s2->Display(11, 12);
    output = t1->Display(7, 12);
    output = w1->Display(99, 765);

    delete r1;  r1 = NULL;
    delete s1;  s1 = NULL;
    delete s2;  s2 = NULL;
    delete s3;  s3 = NULL;
    delete t1;  t1 = NULL;
    delete t2;  t2 = NULL;
    delete w1;  w1 = NULL;

    return 0;

}
