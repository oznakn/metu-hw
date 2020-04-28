// This file will be overwritten while running and evaluating, do not modify!
#include <cmath>
#include <limits>
#include "HashUtils.h"

const int Primes[] = { 353, 431, 521, 631, 761, 919,
                       1103, 1327, 1597, 1931, 2333, 2801, 3371, 4049, 4861, 5839, 7013, 8419, 10103, 12143, 14591,
                       17519, 21023, 25229, 30293, 36353, 43627, 52361, 62851, 75431, 90523, 108631, 130363, 156437,
                       187751, 225307, 270371, 324449, 389357, 467237, 560689, 672827, 807403, 968897, 1162687, 1395263,
                       1674319, 2009191, 2411033, 2893249, 3471899, 4166287, 4999559, 5999471, 7199369, 9270769, 14000839};

#define A 54059
#define B 76963
#define C 86969
unsigned UniversalHash(const std::string data, int size) {
    unsigned h = 31 /* also prime */;
    for (int i = 0; i < size; ++i) {
        h = (h * A) ^ (data[i] * B);
    }
    return h;
}

int Hash(std::string key) {
    return (UniversalHash(key, key.length()) & 0x7FFFFFFF);
}

template<class T, int N>
static int length(const T (&arr)[N]) {
    return N;
}

static bool isPrime(int num) {
    if ((num & 1) != 0) {
        int limit = static_cast<int>(std::sqrt(num));
        for (int divisor = 3; divisor <= limit; divisor += 2) {
            if ((num % divisor) == 0)
                return false;
        }
        return true;
    }
    return (num == 2);
}

int NextCapacity(int from) {
    if (from > Primes[length(Primes) - 1]) {
        for (int i = (from | 1); i < std::numeric_limits<int>::max(); i += 2) {
            if (isPrime(i) && ((i - 1) % 101 != 0))
                return i;
        }

        return from;
    }

    for (int i = 0; i < length(Primes); ++i) {
        if (Primes[i] > from) {
            return Primes[i];
        }
    }

    return -1; // Never happens
}