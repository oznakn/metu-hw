#ifndef HW3_BANK_H
#define HW3_BANK_H
#include "Account.h"
#include <iostream>
#include <string>

/* DO NOT EDIT HEADER FILE*/
class Bank {
private:
    std::string _bank_name;
    int _user_count;
    Account* _users;
public:

    /**
     * Empty constructor
     * give the bank_name as "not_defined"
     * give nullptr for pointers
     * give 0 as _users_count
     */
    Bank();
    /**
     * Constructor
     *
     *
     * @param bank_name name of this bank
     * @param users pointer to hold users of this bank
     * @param user_count number of users this bank has
    */
    Bank(std::string bank_name, Account* const users, int user_count);
    /**
     * Destructor
     *
     * Do not forget to free the space you have created(This assignment does not use smart pointers)
     */
    ~Bank();
    /**
     * Copy constructor(Deep copy)
     *
     * @param rhs The Bank to be copied
     */
    Bank(const Bank& rhs);
    /**
     * You should deep copy the content of the second bank
     * Merge two banks
     * If both banks has a user with the same id, Transactions of these users will be merged in to the same Account
     * For example:
     * Bank1 has [1,2] id users
     * Bank2 has [2,3] id users
     *
     * Bank1 after += operator will have [1,2,3] id users
     * User with id 2 will have its transactions histories merged
     *
     * Transactions with of the users with the same id should be merged and updated
     * @param rhs Merged Bank
     * @return this Bank
     */
    Bank& operator+=(const Bank& rhs);

    /**
     * Add a new account to Bank
     *
     * If the newly added user already exists in this Bank merge their Transactions
     *
     * @param new_acc new account to add to bank
     * @return this Bank
     */
    Bank& operator+=(const Account& new_acc);

    /** Indexing operator overload
     *
     * Return the Account with the given id
     *
     * If there is no Account with the given id return the first element
     *
     * @param account_id id of the Account
     * @return if given id exist in the bank return the account, else return an account with id=-1
     *
     */
    Account& operator[](uint account_id);

    /**
     * Stream overload.
     * all the accounts will be between 01-01-2019 and 31-12-2019
     * What to stream
     * bank_name"tab"number of users who are eligible for a loan"tab"total balance of the bank
     *
     * A user is safe for a loan if and only if that user did not have any negative balance for 2 or more consecutive months
     * For example, let's say our bank named as "banana" has two users
     *
     * User A's balance for each month is as given
     *
     * January - 0
     * February - 0
     * March - 100
     * April - -20
     * May - -30
     * June - 40
     * July - 60
     * August - 0
     * September - 0
     * October - 0
     * November - 0
     * December - 0
     *
     * This user is not eligible because in April and May his/her balance was negative(consecutive)
     * You still have to add 150 to the total balance of the bank
     * User B's balance for each month is as given
     *
     * January - 0
     * February - 0
     * March - 100
     * April - -20
     * May - 40
     * June - -30
     * July - 60
     * August - 0
     * September - 0
     * October - 0
     * November - 0
     * December - 0
     *
     * This user is eligible because negative balances were not consecutive
     * You will also add 150 to the total balance of the bank
     *
     * your output will be as
     * banana   1   300
     */
    friend std::ostream& operator<<(std::ostream& os, const Bank& bank);
};


#endif // HW3_BANK_H
