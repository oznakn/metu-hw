#include "the4.h"

// do not add extra libraries here


#define DEBUG
#define DEBUG_STDERR(x) do { std::cerr << (x) << endl; } while(0)
#define DEBUG_ARRAY(a, n) do { for (int i = 0; i < n; i++) std::cerr << a[i] << " "; std::cerr << endl; } while(0)

// for example usage of DEBUG macros check test.cpp


std::pair<int,int> Assembly_Rec_Helper(int*& a1, int*& a2, int n, int*& t1, int*& t2, int e1, int e2, int x1, int x2, int current_cost, int current_line, int index) {
    int task_cost = current_line == 1 ? a1[index] : a2[index];

    if (index == n - 1) return {current_cost + task_cost + (current_line == 1 ? x1 : x2), 1};

    std::pair<int,int> result1 = Assembly_Rec_Helper(a1, a2, n, t1, t2, e1, e2, x1, x2, task_cost + (current_line == 1 ? 0 : t2[index]), 1, index + 1);
    std::pair<int,int> result2 = Assembly_Rec_Helper(a1, a2, n, t1, t2, e1, e2, x1, x2, task_cost + (current_line == 2 ? 0 : t1[index]), 2, index + 1);

    return {(result1.first < result2.first ? result1.first : result2.first) + current_cost, result1.second + result2.second + 1};
}

std::pair<int,int> Assembly_Rec(int*& a1, int*& a2, int n, int*& t1, int*& t2, int e1, int e2, int x1, int x2) {
    std::pair<int,int> result1 = Assembly_Rec_Helper(a1, a2, n, t1, t2, e1, e2, x1, x2, e1, 1, 0);
    std::pair<int,int> result2 = Assembly_Rec_Helper(a1, a2, n, t1, t2, e1, e2, x1, x2, e2, 2, 0);

    return {result1.first < result2.first ? result1.first : result2.first, result1.second + result2.second};
}


std::pair<int,int> Assembly_Memo_Helper(int mem1[], int mem2[], int*& a1, int*& a2, int n, int*& t1, int*& t2, int e1, int e2, int x1, int x2, int current_cost, int current_line, int index) {
    int task_cost = current_line == 1 ? a1[index] : a2[index];

    if (index == n - 1) {
        int base_value = task_cost + (current_line == 1 ? x1 : x2);

        if (current_line == 1) {
            mem1[index] = base_value;
        } else {
            mem2[index] = base_value;
        }

        return {current_cost + base_value, 1};
    }

    int switch_cost_from_1 = current_line == 1 ? 0 : t2[index];
    int switch_cost_from_2 = current_line == 2 ? 0 : t1[index];

    std::pair<int,int> result1, result2;

    if (mem1[index + 1] != -1) {
        result1 = {task_cost + switch_cost_from_1 + mem1[index + 1], 1};
    } else {
        result1 = Assembly_Memo_Helper(mem1, mem2, a1, a2, n, t1, t2, e1, e2, x1, x2, task_cost + switch_cost_from_1, 1, index + 1);
    }

    if (mem2[index + 1] != -1) {
        result2 = {task_cost + switch_cost_from_2 + mem2[index + 1], 1};
    } else {
        result2 = Assembly_Memo_Helper(mem1, mem2, a1, a2, n, t1, t2, e1, e2, x1, x2, task_cost + switch_cost_from_2, 2, index + 1);
    }

    int result_value = result1.first < result2.first ? result1.first : result2.first;

    if (current_line == 1) {
        mem1[index] = result_value;
    } else {
        mem2[index] = result_value;
    }

    return {result_value + current_cost, result1.second + result2.second + 1};
}

std::pair<int,int> Assembly_Memo(int*& a1, int*& a2, int n, int*& t1, int*& t2, int e1, int e2, int x1, int x2) {
    int mem1[n];
    int mem2[n];

    for (int i = 0; i < n; i++) {
        mem1[i] = -1;
        mem2[i] = -1;
    }

    std::pair<int,int> result1 = Assembly_Memo_Helper(mem1, mem2, a1, a2, n, t1, t2, e1, e2, x1, x2, e1, 1, 0);
    std::pair<int,int> result2 = Assembly_Memo_Helper(mem1, mem2, a1, a2, n, t1, t2, e1, e2, x1, x2, e2, 2, 0);

    return {result1.first < result2.first ? result1.first : result2.first, result1.second + result2.second};
}


std::pair<int,int> Assembly_Tab(int*& a1, int*& a2, int n, int*& t1, int*& t2, int e1, int e2, int x1, int x2) {
    int mem1[n];
    int mem2[n];

    mem1[0] = e1 + a1[0];
    mem2[0] = e2 + a2[0];

    for (int i = 1; i < n; i++) {
        mem1[i] = std::min(mem1[i - 1], mem2[i - 1] + t2[i - 1]) + a1[i];
        mem2[i] = std::min(mem2[i - 1], mem1[i - 1] + t1[i - 1]) + a2[i];
    }

    int val1 = mem1[n - 1] + x1;
    int val2 = mem2[n - 1] + x2;

    return {val1 < val2 ? val1 : val2, n};
}
