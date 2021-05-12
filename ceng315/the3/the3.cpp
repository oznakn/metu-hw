#include "the3.h"
// do not add extra libraries here


/*
    arr       : array to be sorted, in order to get points this array should contain be in sorted state before returning
    size      : size of arr
    ascending : true for ascending, false for descending

    you can use ceil function from cmath

*/

int CutNumber(long k, int s, int l) {
    k = k / pow(10, s);

    return k % ((int) pow(10, l));
}

int CountingSort(long arr[], bool ascending, int n, int s, int l, int k) {
    int result = 0;

    long *B = new long[n];
    int C[k] = {0};

    for (int i = 0; i < n; i++) {
        int index = CutNumber(arr[i], s, l);
        C[index] = C[index] + 1;

        result++;
    }

    for (int index = 1; index < k; index++) {
        C[index] = C[index] + C[index - 1];

        result++;
    }

    if (ascending) {
        for (int i = n - 1; i >= 0; i--) {
            int index = CutNumber(arr[i], s, l);

            if (C[index] > 0) {
                B[C[index] - 1] = arr[i];
                C[index]--;
            }

            result++;
        }
    } else {
        for (int i = 0; i < n; i++) {
            int index = CutNumber(arr[i], s, l);

            if (C[index] > 0) {
                B[n - C[index]] = arr[i];
                C[index]--;
            }

            result++;
        }
    }

    for (int i = 0; i < n; i++) {
        arr[i] = B[i];
        result++;
    }

    delete[] B;

    return result;
}


int RadixSort(long arr[], bool ascending, int n, int l) {
    int result = 0;
    int k = pow(10, l);

    for (int s = 0; s < 12; s += l) {
        result += CountingSort(arr, ascending, n, s, l, k);
    }

    return result;
}
