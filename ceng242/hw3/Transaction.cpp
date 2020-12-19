#include "Transaction.h"

Transaction::Transaction() {
    _amount = -1;
    _date = -1;
}

Transaction::Transaction(double amount, time_t date) {
    _amount = amount;
    _date = date;
}

Transaction::Transaction(const Transaction& rhs) {
    _amount = rhs._amount;
    _date = rhs._date;
}

bool Transaction::operator<(const Transaction& rhs) const {
    return _date < rhs._date;
}

bool Transaction::operator>(const Transaction& rhs) const {
    return _date > rhs._date;
}

bool Transaction::operator<(const time_t date) const {
    return _date < date;
}

bool Transaction::operator>(const time_t date) const {
    return _date > date;
}

double Transaction::operator+(const Transaction& rhs) {
    return _amount + rhs._amount;
}

double Transaction::operator+(const double add) {
    return _amount + add;
}

Transaction& Transaction::operator=(const Transaction& rhs) {
    _amount = rhs._amount;
    _date = rhs._date;

    return *this;
}

std::ostream& operator<<(std::ostream& os, const Transaction& transaction) {
    struct tm *timeinfo = localtime(&transaction._date);

    os << transaction._amount;
    os << "\t-\t";
    os << timeinfo->tm_hour << ":" << timeinfo->tm_min << ":" << timeinfo->tm_sec;
    os << "-";
    os << timeinfo->tm_mday << "/" << (timeinfo->tm_mon + 1) << "/" << (1900 + timeinfo->tm_year);
    os << std::endl;

    return os;
}
