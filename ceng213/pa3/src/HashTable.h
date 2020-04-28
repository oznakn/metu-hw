#ifndef __HASHTABLE__
#define __HASHTABLE__

#include "HashUtils.h"

// Do not modify the public interface of this class.
// Otherwise, your code will note compile!
template <class T>
class HashTable {
    struct Entry {
        std::string Key;             // the key of the entry
        T Value;   // the value of the entry
        bool Deleted;        // flag indicating whether this entry is deleted
        bool Active;         // flag indicating whether this item is currently used

        Entry() : Key(), Value(), Deleted(false), Active(false) {}
    };

    struct Bucket {
        Entry entries[3];
    };

    int _capacity; // INDICATES THE SIZE OF THE TABLE
    int _size; // INDICATES THE NUMBER OF ITEMS IN THE TABLE

    Bucket* _table; // HASH TABLE

    // You can define private methods and variables
    void resizeIfNecessary();
    Entry *fetchEntryIfExists(std::string) const;

public:
    // TODO: IMPLEMENT THESE FUNCTIONS.
    // CONSTRUCTORS, ASSIGNMENT OPERATOR, AND THE DESTRUCTOR
    HashTable();
    HashTable(const HashTable<T>& rhs);
    HashTable<T>& operator=(const HashTable<T>& rhs);
    ~HashTable();

    // TODO: IMPLEMENT THIS FUNCTION.
    // INSERT THE ENTRY IN THE HASH TABLE WITH THE GIVEN KEY & VALUE
    // IF THE GIVEN KEY ALREADY EXISTS, THE NEW VALUE OVERWRITES
    // THE ALREADY EXISTING ONE.
    // IF LOAD FACTOR OF THE TABLE IS BIGGER THAN 0.5,
    // RESIZE THE TABLE WITH THE NEXT PRIME NUMBER.
    void Insert(std::string key, const T& value);

    // TODO: IMPLEMENT THIS FUNCTION.
    // DELETE THE ENTRY WITH THE GIVEN KEY FROM THE TABLE
    // IF THE GIVEN KEY DOES NOT EXIST IN THE TABLE, JUST RETURN FROM THE FUNCTION
    // HINT: YOU SHOULD UPDATE ACTIVE & DELETED FIELDS OF THE DELETED ENTRY.
    void Delete(std::string key);

    // TODO: IMPLEMENT THIS FUNCTION.
    // IT SHOULD RETURN THE VALUE THAT CORRESPONDS TO THE GIVEN KEY.
    // IF THE KEY DOES NOT EXIST, THIS FUNCTION MUST RETURN T()
    T Get(std::string key) const;

    // TODO: IMPLEMENT THIS FUNCTION.
    // AFTER THIS FUNCTION IS EXECUTED THE TABLE CAPACITY MUST BE
    // EQUAL TO newCapacity AND ALL THE EXISTING ITEMS MUST BE REHASHED
    // ACCORDING TO THIS NEW CAPACITY.
    // WHEN CHANGING THE SIZE, YOU MUST REHASH ALL OF THE ENTRIES FROM 0TH ENTRY TO LAST ENTRY
    void Resize(int newCapacity);

    // TODO: IMPLEMENT THIS FUNCTION.
    // RETURNS THE AVERAGE NUMBER OF PROBES FOR SUCCESSFUL SEARCH
    double getAvgSuccessfulProbe();

    // TODO: IMPLEMENT THIS FUNCTION.
    // RETURNS THE AVERAGE NUMBER OF PROBES FOR UNSUCCESSFUL SEARCH
    double getAvgUnsuccessfulProbe();

    // THE IMPLEMENTATION OF THESE FUNCTIONS ARE GIVEN TO YOU
    // DO NOT MODIFY!
    int Capacity() const;
    int Size() const;
};



template <class T>
HashTable<T>::HashTable() {
    this->_capacity = 0;
    this->_size = 0;
    this->_table = NULL;
}

template <class T>
HashTable<T>::HashTable(const HashTable<T>& rhs) {
    this->_capacity = rhs._capacity;
    this->_size = rhs._size;

    this->_table = new Bucket[rhs._capacity];

    for (int i = 0; i < this->_size; i++) {
        for (int j = 0; j < 3; j++) {
            this->_table[i].entries[j].Deleted = rhs._table[i].entries[j].Deleted;
            this->_table[i].entries[j].Active = rhs._table[i].entries[j].Active;

            if (rhs._table[i].entries[j].Active) {
                this->_table[i].entries[j].Key = rhs._table[i].entries[j].Key;
                this->_table[i].entries[j].Value = rhs._table[i].entries[j].Value;
            }
        }
    }
}

template <class T>
HashTable<T>& HashTable<T>::operator=(const HashTable<T>& rhs) {
    if (this->_table != NULL) {
        delete [] this->_table;
    }

    this->_capacity = rhs._capacity;
    this->_size = rhs._size;

    this->_table = new Bucket[rhs._capacity];

    for (int i = 0; i < this->_size; i++) {
        for (int j = 0; j < 3; j++) {
            this->_table[i].entries[j].Deleted = rhs._table[i].entries[j].Deleted;
            this->_table[i].entries[j].Active = rhs._table[i].entries[j].Active;

            if (rhs._table[i].entries[j].Active) {
                this->_table[i].entries[j].Key = rhs._table[i].entries[j].Key;
                this->_table[i].entries[j].Value = rhs._table[i].entries[j].Value;
            }
        }
    }
}

template <class T>
HashTable<T>::~HashTable() {
    if (this->_table != NULL) {
        delete [] this->_table;
    }
}

template <class T>
void HashTable<T>::resizeIfNecessary() {
    if (this->_capacity == 0 || (this->_size / (3 * ((double) this->_capacity))) > 0.5) {
        int next_capacity = NextCapacity(this->_capacity);

        this->Resize(next_capacity);
    }
}

template <class T>
typename HashTable<T>::Entry *HashTable<T>::fetchEntryIfExists(std::string key) const {
    if (this->_table == NULL) {
        return NULL;    
    }
    
    int hash = Hash(key);
    int q = 0;

    while ((q*q) < this->_capacity) {
        int pos = (hash + (q*q)) % this->_capacity;

        for (int i = 0; i < 3; i++) {
            if (this->_table[pos].entries[i].Active == false && this->_table[pos].entries[i].Deleted == false) {
                return NULL;
            }
            else if (this->_table[pos].entries[i].Active && this->_table[pos].entries[i].Key == key) {
                return &(this->_table[pos].entries[i]);
            }
        }

        q++;
    }

    return NULL;
}

template <class T>
void HashTable<T>::Insert(std::string key, const T& value) {
    if (this->_capacity == 0) {
        this->_capacity = NextCapacity(this->_capacity);
        this->_table = new Bucket[this->_capacity];
    }

    Entry *entry = this->fetchEntryIfExists(key);

    if (entry != NULL) {
        entry->Key = key;
        entry->Value = value;

        entry->Active = true;
        entry->Deleted = false;
    }
    else {
        this->resizeIfNecessary();

        int hash = Hash(key);
        int q = 0;

        bool hasPlaced = false;

        while (!hasPlaced) {
            int pos = (hash + (q*q)) % this->_capacity;

            for (int i = 0; i < 3; i++) {
                if (this->_table[pos].entries[i].Active == false) {
                    this->_table[pos].entries[i].Key = key;
                    this->_table[pos].entries[i].Value = value;

                    this->_table[pos].entries[i].Active = true;
                    this->_table[pos].entries[i].Deleted = false;

                    hasPlaced = true;
                    break;
                }
            }

            q++;
        }

        if (hasPlaced) {
            this->_size++;
        }
    }
}

template <class T>
void HashTable<T>::Delete(std::string key) {
    Entry *entry = this->fetchEntryIfExists(key);

    if (entry != NULL) {
        entry->Active = false;
        entry->Deleted = true;

        this->_size--;
    }
}

template <class T>
T HashTable<T>::Get(std::string key) const {
    HashTable<T>::Entry *entry = this->fetchEntryIfExists(key);

    if (entry != NULL) {
        return entry->Value;
    }

    return T();
}

template <class T>
void HashTable<T>::Resize(int new_capacity) {
    Bucket* new_table = new Bucket[new_capacity];
    
    if (this->_table != NULL) {
        for (int i = 0; i < this->_capacity; i++) {
            for (int j = 0; j < 3; j++) {
                if (this->_table[i].entries[j].Active) {
                    int hash = Hash(this->_table[i].entries[j].Key);
                    int q = 0;
            
                    bool hasPlaced = false;
            
                    while (!hasPlaced) {
                        int pos = (hash + (q*q)) % new_capacity;
            
                        for (int k = 0; k < 3; k++) {
                            if (new_table[pos].entries[k].Active == false) {
                                new_table[pos].entries[k].Key = this->_table[i].entries[j].Key;
                                new_table[pos].entries[k].Value = this->_table[i].entries[j].Value;
            
                                new_table[pos].entries[k].Active = true;
                                new_table[pos].entries[k].Deleted = false;
            
                                hasPlaced = true;
                                break;
                            }
                        }
            
                        q++;
                    }
                }
            }
        }
    }

    this->_capacity = new_capacity;
    delete [] this->_table;
    this->_table = new_table;
}


template <class T>
double HashTable<T>::getAvgSuccessfulProbe() {
    int result = 0;

    for (int i = 0; i < this->_capacity; i++) {
        for (int j = 0; j < 3; j++) {
            if (this->_table[i].entries[j].Active) {
                std::string key = this->_table[i].entries[j].Key;
                int hash = Hash(key);

                int q = 0;
                bool found = false;

                while (!found && (q*q) < this->_capacity) {
                    int pos = (hash + (q*q)) % this->_capacity;
                    result++;

                    for (int k = 0; k < 3; k++) {
                        if (this->_table[pos].entries[k].Active && this->_table[pos].entries[k].Key == key) {
                            found = true;
                            break;
                        }
                    }

                    q++;
                }
            }
        }
    }

    return (result / (double) this->_size);
}

template <class T>
double HashTable<T>::getAvgUnsuccessfulProbe() {
    int result = 0;

    for (int hash = 0; hash < this->_capacity; hash++) {
        int q = 0;
        bool found = false;

        while (!found && (q*q) < this->_capacity) {
            int pos = (hash + (q*q)) % this->_capacity;
            result++;

            for (int i = 0; i < 3; i++) {
                if (this->_table[pos].entries[i].Active == false) {
                    found = true;
                    break;
                }
            }

            q++;
        }
    }

    return (result / (double) this->_capacity);
}

template <class T>
int HashTable<T>::Capacity() const {
    return _capacity;
}

template <class T>
int HashTable<T>::Size() const {
    return _size;
}

#endif
