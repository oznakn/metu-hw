#include "the4.h"

void print_pair(std::pair<int,int> result, const char *right_answer) {
    std::cout << "<" << result.first << ", " << result.second << ">" << std::endl;
    std::cout << right_answer << std::endl << std::endl;
}

int main() {
    int *a1, *a2, *t1, *t2;
    std::pair<int,int> result;

    int test1_a1[] = {7, 9, 3, 4, 8};
    int test1_a2[] = {8, 5, 6, 4, 5};
    int test1_n = 5;
    int test1_t1[] = {2, 3, 1, 3};
    int test1_t2[] = {2, 1, 2, 2};
    int test1_e1 = 2;
    int test1_e2 = 4;
    int test1_x1 = 3;
    int test1_x2 = 6;

    a1 = test1_a1;
    a2 = test1_a2;
    t1 = test1_t1;
    t2 = test1_t2;

    print_pair(Assembly_Rec(a1, a2, test1_n, t1, t2, test1_e1, test1_e2, test1_x1, test1_x2), "<35, 62>");
    print_pair(Assembly_Memo(a1, a2, test1_n, t1, t2, test1_e1, test1_e2, test1_x1, test1_x2), "<35, 18>");
    print_pair(Assembly_Tab(a1, a2, test1_n, t1, t2, test1_e1, test1_e2, test1_x1, test1_x2), "<35, 5>");

    int test2_a1[] = {4, 5, 3, 2};
    int test2_a2[] = {2, 10, 1, 4};
    int test2_n = 4;
    int test2_t1[] = {7,4,5};
    int test2_t2[] = {9,2,8};
    int test2_e1 = 10;
    int test2_e2 = 12;
    int test2_x1 = 18;
    int test2_x2 = 7;

    a1 = test2_a1;
    a2 = test2_a2;
    t1 = test2_t1;
    t2 = test2_t2;

    print_pair(Assembly_Rec(a1, a2, test2_n, t1, t2, test2_e1, test2_e2, test2_x1, test2_x2), "<35, 30>");
    print_pair(Assembly_Memo(a1, a2, test2_n, t1, t2, test2_e1, test2_e2, test2_x1, test2_x2), "<35, 14>");
    print_pair(Assembly_Tab(a1, a2, test2_n, t1, t2, test2_e1, test2_e2, test2_x1, test2_x2), "<35, 4>");

    int test3_a1[] = {4, 5};
    int test3_a2[] = {2, 10};
    int test3_n = 2;
    int test3_t1[] = {2};
    int test3_t2[] = {1};
    int test3_e1 = 2;
    int test3_e2 = 1;
    int test3_x1 = 1;
    int test3_x2 = 1;

    a1 = test3_a1;
    a2 = test3_a2;
    t1 = test3_t1;
    t2 = test3_t2;

    print_pair(Assembly_Rec(a1, a2, test3_n, t1, t2, test3_e1, test3_e2, test3_x1, test3_x2), "<10, 6>");
    print_pair(Assembly_Memo(a1, a2, test3_n, t1, t2, test3_e1, test3_e2, test3_x1, test3_x2), "<10, 6>");
    print_pair(Assembly_Tab(a1, a2, test3_n, t1, t2, test3_e1, test3_e2, test3_x1, test3_x2), "<10, 2>");

    return 0;
}
