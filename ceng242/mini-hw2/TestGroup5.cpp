#include "Book.h"
#include "SoftCopy.h"

// TestGroup 5 : "SoftCopy Operator Overload: operator+() Tests"

// COMPILE: g++ -std=c++11 TestGroup5.cpp Book.cpp  SoftCopy.cpp -o test
// RUN: valgrind --leak-check=full ./test

int main() {

    /************************ Construct the books ************************/

    // Create SoftCopies
    SoftCopy* s1 = new SoftCopy(540, 12.0, 0, 18);      // 19 sayfalık kitap
    SoftCopy* s2 = new SoftCopy(540, 8.6, 10, 14);      // 5 sayfalık parça
    SoftCopy* s3 = new SoftCopy(540, 17.0, 4, 6);       // 3 sayfalık parça

    SoftCopy* r1 = new SoftCopy(520, 15.0, 0, 7);       // pow(2,3)= 8 sayfalık kitap
    SoftCopy* r2 = new SoftCopy(520, 10.0, 4, 4);

    SoftCopy* t1 = new SoftCopy(530, 16.5, 0, 15);      // pow(2,4) = 16 sayfalık kitap
    SoftCopy* t2 = new SoftCopy(530, 2, 10, 11);        // 2 sayfalık exact branch

    SoftCopy* w1 = new SoftCopy(550, 45.8, 0, 1025);    // Large book
    SoftCopy* w2 = new SoftCopy(550, 9.1, 49, 399);

    SoftCopy* k1 = new SoftCopy(590, 21.0, 8, 23);
    SoftCopy* k2 = new SoftCopy(590, 7.5, 18, 20);

    /********************* Prepare softcopy page content ******************/
    string s_pages[20];
    for (int i=0; i<20; i++) {
        string part1 = "PAGE CONTENT ";
        string part2 = ", in SOFT COPY 540";
        s_pages[i] = part1 + to_string(i) + part2;
    }

    string r_pages[8];
    for (int i=0; i<8; i++) {
        string part1 = "PAGE CONTENT ";
        string part2 = ", in SOFT COPY 520";
        r_pages[i] = part1 + to_string(i) + part2;
    }

    string t_pages[16];
    for (int i=0; i<16; i++) {
        string part1 = "PAGE CONTENT ";
        string part2 = ", in SOFT COPY 530";
        t_pages[i] = part1 + to_string(i) + part2;
    }

    string w_pages[10];
    int w_page_ids[10] = {825, 417, 100, 9, 310, 781, 615, 986, 199, 299};
    for (int i=0; i<10; i++) {
        string part1 = "PAGE CONTENT ";
        string part2 = ", in SOFT COPY 550";
        w_pages[i] = part1 + to_string(w_page_ids[i]) + part2;
    }

    string k_pages[24];
    for (int i=0; i<24; i++) {
        string part1 = "PAGE CONTENT ";
        string part2 = ", in SOFT COPY 590";
        k_pages[i] = part1 + to_string(i) + part2;
    }

    /****************************** Upload Pages ****************************/
    // Upload only one page into r2,
    r2->UploadPage(r_pages[4], 4);

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

    // Upload specific pages of w2
    w2->UploadPage(w_pages[8], w_page_ids[8]);
    w2->UploadPage(w_pages[9], w_page_ids[9]);

    // Upload specific pages of k1
    for (int i=8; i<24; i++) {
        if (i==19)
            continue;
        k1->UploadPage(k_pages[i], i);
    }
    k2->UploadPage(k_pages[19], 19);

    /************************ Summation Tests ***********************/

    SoftCopy& t12 = *t1 + *t2;
    SoftCopy& r12 = *r1 + *r2;
    SoftCopy& k12 = *k1 + *k2;
    SoftCopy& s12 = *s1 + *s2;
    SoftCopy& s123 = s12 + *s3;
    SoftCopy& w12 = *w1 + *w2;

    /************************ Delete the softcopies ***********************/
    delete s1;  s1 = NULL;
    delete s2;  s2 = NULL;
    delete s3;  s3 = NULL;
    delete r1;  r1 = NULL;
    delete r2;  r2 = NULL;
    delete t1;  t1 = NULL;
    delete t2;  t2 = NULL;
    delete w1;  w1 = NULL;
    delete w2;  w2 = NULL;
    delete k1;  k1 = NULL;
    delete k2;  k2 = NULL;
    delete &t12;
    delete &r12;
    delete &k12;
    delete &w12;
    delete &s12;
    delete &s123;

    return 0;
}
