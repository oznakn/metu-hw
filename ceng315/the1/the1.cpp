#include "the1.h"

// do not add extra libraries here


int Merge (int*& A, int p, int q, int r, bool ascending)
{
    int result = 0;

    int n1 = q - p + 1;
    int n2 = r - q;

    int L[n1 + 1], R[n2 + 1];

    for (int i = 0; i < n1; i++) {
        L[i] = A[p + i];
    }

    for (int i = 0; i < n2; i++) {
        R[i] = A[q + 1 + i];
    }

    L[n1] = ascending ? INT_MAX : INT_MIN;
    R[n2] = ascending ? INT_MAX : INT_MIN;

    int x = 0;
    int y = 0;

    for (int i = p; i <= r; i++) {
        if (x != n1 && y != n2) {
            result++;
        }

        if ((ascending && L[x] <= R[y]) || (!ascending && L[x] >= R[y])) {
            A[i] = L[x++];
        } else {
            A[i] = R[y++];
        }
    }

    return result;
}

int MergeSortHelper (int*& arr, int p, int r, bool ascending)
{
    if (p < r) {
        int q = (p+r)/2;

        int x = MergeSortHelper(arr, p, q, ascending);
        int y = MergeSortHelper(arr, q + 1, r, ascending);

        return x + y + Merge(arr, p, q, r, ascending);
    }

    return 0;
}

/*
    arr       : array to be sorted, in order to get points this array should contain be in sorted state before returning
    size      : size of arr
    ascending : true for ascending, false for descending

    you can use ceil function from cmath

*/
int MergeSort (int*& arr, int size, bool ascending)
{
    return MergeSortHelper(arr, 0, size - 1, ascending);
}


int FunkySortHelper (int*& arr, int start, int end, bool ascending)
{
    int curr_size = end - start + 1;

    int result = 0;

    if (curr_size > 1) {
        result += 1;

        if ((ascending && arr[start] > arr[end]) || (!ascending && arr[start] < arr[end])) {
            int temp = arr[start];
            arr[start] = arr[end];
            arr[end] = temp;
        }
    }

    if (curr_size >= 3) {
        int cut_amount = ceil(curr_size * (2.0/3.0));

        int x = FunkySortHelper(arr, start, start + cut_amount - 1, ascending);
        int y = FunkySortHelper(arr, end - cut_amount + 1, end, ascending);
        int z = FunkySortHelper(arr, start, start + cut_amount - 1, ascending);

        result += x + y + z;
    }

    return result;
}

int FunkySort (int*& arr, int size, bool ascending)
{
    return FunkySortHelper(arr, 0, size - 1, ascending);
}
