// This file will be overwritten while running and evaluating, do not modify!
#ifndef __HASHUTILS__
#define __HASHUTILS__
#include <string>
/*
 * Returns the hash value of the given key
 */
int Hash(std::string key);

/*
 * Finds the next appropriate hash table size from a given number
 */
int NextCapacity(int prev);

#endif