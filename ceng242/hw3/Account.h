#ifndef HW3_ACCOUNT_H
#define HW3_ACCOUNT_H

#include <iostream>
#include <ctime>
#include "Transaction.h"

/* DO NOT EDIT HEADER FILE*/

class Account {
private:
    int _id;
    Transaction** _activity;
    int* _monthly_activity_frequency;


public:
    /**
     * Empty constructor
     * give the id as -1
     * give nullptr for pointers
     */
    Account();
    /**
     * Constructor
     *
     *
     * Note: The given activity array will have 12 Transaction*
     * Each of these Transaction* will represent a month from the 2019
     * Basicaly activity[0] will represent January
     *          activity[11] will represent February
     *          activity[11] will represent March
     *                      ...
     *          activity[10] will represent November
     *          activity[11] will represent December
     * activity[0] will only contain Transactions happened in January
     * However, be careful that Transactions inside of activity[i] will not be in sorted order
     * For Example: We are certain that activity[0] is containing Transactions happened in January 2019
     * But we are not sure which of them happened first.
     * I strongly suggest you to use a sorting algorithm while storing these Transaction to your object.
     * (Sorting by the date, So that you can directly use them in stream overload)
     * (You can use bubble sort)
     *
     * @param id id of this Account
     * @param activity 2d Transaction array first layers lenght is 12 for each month
     * @param monthly_activity_frequency how many transactions made in each month
    */
    Account(int id, Transaction** const activity, int* monthly_activity_frequency);

    /**
     * Destructor
     *
     * Do not forget to free the space you have created(This assignment does not use smart pointers)
     */
    ~Account();

    /**
     * Copy constructor(Deep copy)
     *
     * @param other The Account to be copied
     */
    Account(const Account& rhs);

    /**
     * Copy constructor(Deep copy)
     *
     * This copy constructors takes two time_t elements
     * Transactions of the old Account will be copied to new Account
     * if and only if they are between these given dates
     * Given dates will not be included.
     *
     * @param rhs The Account to be copied
     * @param start_date Starting date for transaction to be copied.
     * @param end_date Ending date for transactions to be copied.
     */
    Account(const Account& rhs, time_t start_date, time_t end_date);

    /**
     * Move constructor
     *
     * @param rhs Account which you will move the resources from
     */
    Account(Account&& rhs);

    /**
     * Move assignment operator
     *
     * @param rhs Account which you will move the resources from
     */
    Account& operator=(Account&& rhs);

    /**
     * Assignment operator
     * deep copy
     *
     * @param rhs Account to assign
     * @return this account
     */
    Account& operator=(const Account& rhs);
    /**
     * Equality comparison overload
     *
     * This operator checks only id of the Account
     *
     * @param rhs The Account to compare
     * @return returns true if both ids are same false othervise
     */

    bool operator==(const Account& rhs) const;
    /**
     * Equality comparison overload
     *
     * This operator checks only id of the Account
     *
     * @param id to compare
     * @return returns true if both ids are same false othervise
     */
    bool operator==(int id) const;


    /**
     * sum and equal operator
     * Add Transactions of two Accounts
     * You have to add transactions in correct places in your _activity array
     * Note: Remember that _activity[0] is always January and _activity[11] is always December
     * (This information also holds for every other month)
     *
     * You can have Transactions with the same date
     *
     * @param rhs Account which take new Transactions from
     * @return this Account after adding new Transactions
     */
    Account& operator+=(const Account& rhs);

    /**
     * How much money Account has(Sum of Transaction amounts)
     *
     *
     * @return total amount of the money of the account
     */
    double balance();

    /**
     * How much money Account has at the end of given date
     *
     * Given date will not be included.
     * @param end_date You will count the amounts until this given date(not inclusive)
     * @return Total amount the Account has until given date
     */
    double balance(time_t end_date);


    /**
     * How much money Account between given dates
     * Given dates will not be included.
     *
     * @param end_date You will count the amounts between given dates(not inclusive)
     * @return Total amount the Account has between given dates
     * You will only count a Transaction amount if and only if it occured between given dates
     */
    double balance(time_t start_date, time_t end_date);





    /**
     * Stream overload.
     *
     *
     *
     * What to stream
     * Id of the user
     * Earliest Transaction amount"tab"-"tab"hour:minute:second-day/month/year(in localtime)
     * Second earliest Transaction amount"tab"-"tab"hour:minute:second-day/month/year(in localtime)
     * ...
     * Latest Transaction amount"tab-tab"hour:minute:second-day/month/year(in localtime)
     *
     * Note: _activity array will only contain dates from January 2019 to December 2019
     * Note: Transactions should be in order by date
     * Note: either of _monthly_activity_frequency or _activity is nullptr
     * you will just stream
     * -1
     * @param os Stream to be used.
     * @param Account to be streamed.
     * @return the current Stream
     */

    friend std::ostream& operator<<(std::ostream& os, const Account& account);
};



#endif //HW3_ACCOUNT_H
