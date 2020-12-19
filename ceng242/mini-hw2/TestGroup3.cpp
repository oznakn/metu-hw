#include "Book.h"
#include "SoftCopy.h"

// TestGroup 3 : "SoftCopy Copy Constructor and Assignment Operator Tests"

// COMPILE: g++ -std=c++11 TestGroup3.cpp Book.cpp  SoftCopy.cpp -o test
// RUN: valgrind --leak-check=full ./test

int main() {

    /************************ Construct the books ************************/

    // Create SoftCopies
    SoftCopy* s1 = new SoftCopy(340, 12.0, 0, 18);      // 19 sayfalık kitap
    SoftCopy* s2 = new SoftCopy(340, 8.6, 10, 14);      // 5 sayfalık parça
    SoftCopy* t1 = new SoftCopy(360, 24.0, 0, 18);      // 19 sayfalık kitap
    SoftCopy* t2 = new SoftCopy(360, 16.6, 10, 14);     // 5 sayfalık parça
    SoftCopy* t3 = new SoftCopy(360, 17.0, 4, 6);       // 3 sayfalık parça

    SoftCopy* r1 = new SoftCopy(320, 15.0, 0, 7);       // pow(2,3)= 8 sayfalık kitap
    SoftCopy* p1 = new SoftCopy(380, 11.0, 0, 7);       // pow(2,3)= 8 sayfalık kitap
    SoftCopy* q1 = new SoftCopy(390, 13.3, 0, 7);       // pow(2,3)= 8 sayfalık kitap
    SoftCopy* w1 = new SoftCopy(310, 60, 3, 9);         // 7 sayfalık kitap, boş kalacak

    SoftCopy* copy_s1 = NULL;
    SoftCopy* copy_s2 = NULL;
    SoftCopy* copy_r1 = NULL;

    /********************* Prepare softcopy page content ******************/
    string s_pages[20], t_pages[20];
    for (int i=0; i<20; i++) {
        string part1 = "PAGE CONTENT ";
        string part2 = ", in SOFT COPY 340";
        string part3 = ", in SOFT COPY 360";
        s_pages[i] = part1 + to_string(i) + part2;
        t_pages[i] = part1 + to_string(i) + part3;
    }

    string r_pages[8], p_pages[8], q_pages[8];
    for (int i=0; i<8; i++) {
        string part1 = "PAGE CONTENT ";
        string part2 = ", in SOFT COPY 320";
        string part3 = ", in SOFT COPY 380";
        string part4 = ", in SOFT COPY 390";
        r_pages[i] = part1 + to_string(i) + part2;
        p_pages[i] = part1 + to_string(i) + part3;
        q_pages[i] = part1 + to_string(i) + part4;
    }

    /************************** Upload Pages **************************/

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

    // Upload specific pages of t1
    t1->UploadPage(t_pages[0], 0);
    t1->UploadPage(t_pages[3], 3);
    t1->UploadPage(t_pages[17], 17);

    // Upload specific pages of t2
    t2->UploadPage(t_pages[10], 10);
    t2->UploadPage(t_pages[11], 11);
    t2->UploadPage(t_pages[12], 12);
    t2->UploadPage(t_pages[13], 13);
    t2->UploadPage(t_pages[14], 14);

    // Upload specific pages of t3
    t3->UploadPage(t_pages[4], 4);
    t3->UploadPage(t_pages[5], 5);
    t3->UploadPage(t_pages[6], 6);

    // Upload all pages of r1, build full tree
    for (int i=0; i<8; i++) {
        r1->UploadPage(r_pages[i], i);
        p1->UploadPage(p_pages[i], i);
        q1->UploadPage(q_pages[i], i);
    }

    copy_s1 = new SoftCopy(*s1);
    copy_s2 = new SoftCopy(*s2);
    copy_r1 = new SoftCopy(*r1);

    /************************** Assignment Tests **************************/
    *t1 = *t2;
    *t3 = *p1;
    *q1 = *w1;

    delete s1;  s1 = NULL;
    delete s2;  s2 = NULL;
    delete r1;  r1 = NULL;
    delete t1;  t1 = NULL;
    delete t2;  t2 = NULL;
    delete t3;  t3 = NULL;
    delete p1;  p1 = NULL;
    delete q1;  q1 = NULL;
    delete w1;  w1 = NULL;
    delete copy_s1; copy_s1 = NULL;
    delete copy_s2; copy_s2 = NULL;
    delete copy_r1; copy_r1 = NULL;


    return 0;
}
