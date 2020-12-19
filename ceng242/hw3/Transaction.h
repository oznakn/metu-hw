#ifndef HW3_TRANSACTION_H
#define HW3_TRANSACTION_H

#include <iostream>
#include <ctime>

/* DO NOT EDIT HEADER FILE*/

class Transaction {
private:
    double _amount;
    time_t _date;



public:
    /**
     * Empty constructor
     * give -1 to everything
     */
    Transaction();
    /**
     * Constructor
     *
     * @param amount The value of Transaction(Can be negative or positive)
     * @param date Transaction date
    */
    Transaction(double amount, time_t date);
    /**
     * Copy Constructor.
     *
     * @param rhs The Transaction to be copied.
     */
    Transaction(const Transaction& rhs);

    /**
     * Compare two Transaction based on their date
     *
     * @param rhs Compared Transaction
     * @return If current Transaction happened before the given Transaction return true
     * else return false
     */
    bool operator<(const Transaction& rhs) const;
    /**
     * Compare two Transaction based on their date
     *
     * @param date Compared date
     * @return If current Transaction happened after the given Transaction return true
     * else return false
     */
    bool operator>(const Transaction& rhs) const;
    /**
     * Compare a Transaction with a given date
     *
     * @param date Compared date
     * @return If current Transaction happened before the given date return true
     * else return false
     */
    bool operator<(const time_t date) const;
    /**
     * Compare a Transaction with a given date
     *
     * @param date Compared date
     * @return If current Transaction happened after the given date return true
     * else return false
     */
    bool operator>(const time_t date) const;


    /**
     * Sum the value of two Transaction amounts
     *
     * @param rhs The transaction to sum over
     * @return The output of the summation in double format
     */
    double operator+(const Transaction& rhs);

    /**
     * Sum the value of a Transaction with another double
     *
     * @param add The amount to sum over
     * @return The output of the summation in double format
     */
    double operator+(const double add);


    /**
     * Assignment operator
     *
     * @param rhs Transaction to assign
     * @return this Transaction
     */
    Transaction& operator=(const Transaction& rhs);

    /**
     * Stream overload
     *
     * What to stream:
     * Transaction amount"tab-tab"hour:minute:second-day/month/year(in localtime)
     *
     * @param os Stream to be used.
     * @param transaction Transaction to be streamed.
     * @return the current Stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Transaction& transaction);


};



#endif //HW3_TRANSACTION_H