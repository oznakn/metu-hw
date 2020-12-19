#include "Account.h"

void sort_transactions(Transaction *transactions, int length) {
    bool is_sorted = false;

    while (!is_sorted) {
        is_sorted = true;

        for (int i = 0; i < length - 1; i++) {
            if (transactions[i] > transactions[i+1]) {
                Transaction temp = transactions[i];
                transactions[i] = transactions[i+1];
                transactions[i+1] = temp;

                is_sorted = false;
            }
        }
    }
}

Account::Account() {
    _id = -1;
    _activity = nullptr;
    _monthly_activity_frequency = nullptr;
}

Account::Account(int id, Transaction** const activity, int* monthly_activity_frequency) {
    _id = id;
    _activity = new Transaction*[12];
    _monthly_activity_frequency = new int[12];

    for (int i = 0; i < 12; i++) {
        _monthly_activity_frequency[i] = monthly_activity_frequency[i];
    }

    for (int i = 0; i < 12; i++) {
        _activity[i] = new Transaction[_monthly_activity_frequency[i]];

        for (int j = 0; j < _monthly_activity_frequency[i]; j++) {
            _activity[i][j] = activity[i][j];
        }

        sort_transactions(_activity[i], _monthly_activity_frequency[i]);
    }
}

Account::~Account() {
    if (_activity != nullptr) {
        for (int i = 0; i < 12; i++) {
            delete [] _activity[i];
        }

        delete [] _activity;
    }

    if (_monthly_activity_frequency != nullptr) {
        delete [] _monthly_activity_frequency;
    }
}

Account::Account(const Account& rhs) {
    _id = rhs._id;
    _activity = new Transaction*[12];
    _monthly_activity_frequency = new int[12];

    for (int i = 0; i < 12; i++) {
        _monthly_activity_frequency[i] = 0;

        if (rhs._monthly_activity_frequency != nullptr) {
            _monthly_activity_frequency[i] = rhs._monthly_activity_frequency[i];
        }
    }

    for (int i = 0; i < 12; i++) {
        _activity[i] = new Transaction[_monthly_activity_frequency[i]];

        for (int j = 0; j < _monthly_activity_frequency[i]; j++) {
            _activity[i][j] = rhs._activity[i][j];
        }

        sort_transactions(_activity[i], _monthly_activity_frequency[i]);
    }
}

Account::Account(const Account& rhs, time_t start_date, time_t end_date) {
    _id = rhs._id;
    _activity = new Transaction*[12];
    _monthly_activity_frequency = new int[12];

    for (int i = 0; i < 12; i++) {
        _monthly_activity_frequency[i] = 0;

        if (rhs._monthly_activity_frequency != nullptr && rhs._activity != nullptr) {
            for (int j = 0; j < rhs._monthly_activity_frequency[i]; j++) {
                if (rhs._activity[i][j] > start_date && rhs._activity[i][j] < end_date) {
                    _monthly_activity_frequency[i]++;
                }
            }
        }
    }

    for (int i = 0; i < 12; i++) {
        _activity[i] = new Transaction[_monthly_activity_frequency[i]];

        int pos = 0;

        if (rhs._monthly_activity_frequency != nullptr && rhs._activity != nullptr) {
            for (int j = 0; j < rhs._monthly_activity_frequency[i]; j++) {
                if (rhs._activity[i][j] > start_date && rhs._activity[i][j] < end_date) {
                    _activity[i][pos] = rhs._activity[i][j];
                    pos++;
                }
            }
        }
        sort_transactions(_activity[i], _monthly_activity_frequency[i]);
    }
}


Account::Account(Account&& rhs) {
    _id = rhs._id;
    _activity = rhs._activity;
    _monthly_activity_frequency = rhs._monthly_activity_frequency;

    rhs._id = -1;
    rhs._activity = nullptr;
    rhs._monthly_activity_frequency = nullptr;
}

Account& Account::operator=(Account&& rhs) {
    if (this == &rhs)
        return *this;

    if (_activity != nullptr) {
        for (int i = 0; i < 12; i++) {
            delete [] _activity[i];
        }

        delete [] _activity;
    }

    if (_monthly_activity_frequency != nullptr) {
        delete [] _monthly_activity_frequency;
    }

    _id = rhs._id;
    _activity = rhs._activity;
    _monthly_activity_frequency = rhs._monthly_activity_frequency;

    rhs._id = -1;
    rhs._activity = nullptr;
    rhs._monthly_activity_frequency = nullptr;

    return *this;
}

Account& Account::operator=(const Account& rhs) {
    if (this == &rhs)
        return *this;

    if (_activity != nullptr) {
        for (int i = 0; i < 12; i++) {
            delete [] _activity[i];
        }

        delete [] _activity;
    }

    if (_monthly_activity_frequency != nullptr) {
        delete [] _monthly_activity_frequency;
    }

    _id = rhs._id;
    _activity = new Transaction*[12];
    _monthly_activity_frequency = new int[12];

    for (int i = 0; i < 12; i++) {
        _monthly_activity_frequency[i] = 0;

        if (rhs._monthly_activity_frequency != nullptr) {
            _monthly_activity_frequency[i] = rhs._monthly_activity_frequency[i];
        }
    }

    for (int i = 0; i < 12; i++) {
        _activity[i] = new Transaction[_monthly_activity_frequency[i]];

        for (int j = 0; j < _monthly_activity_frequency[i]; j++) {
            _activity[i][j] = rhs._activity[i][j];
        }

        sort_transactions(_activity[i], _monthly_activity_frequency[i]);
    }

    return *this;
}

bool Account::operator==(const Account& rhs) const {
    return _id == rhs._id;
}

bool Account::operator==(int id) const {
    return _id == id;
}

Account& Account::operator+=(const Account& rhs) {
    Transaction **new_activity = new Transaction*[12];
    int *new_monthly_activity_frequency = new int[12];

    for (int i = 0; i < 12; i++) {
        new_monthly_activity_frequency[i] = 0;

        if (_monthly_activity_frequency != nullptr) {
            new_monthly_activity_frequency[i] += _monthly_activity_frequency[i];
        }

        if (rhs._monthly_activity_frequency != nullptr) {
            new_monthly_activity_frequency[i] += rhs._monthly_activity_frequency[i];
        }
    }

    for (int i = 0; i < 12; i++) {
        new_activity[i] = new Transaction[new_monthly_activity_frequency[i]];

        int pos = 0;

        if (_activity != nullptr) {
            for (int j = 0; j < _monthly_activity_frequency[i]; j++) {
                new_activity[i][pos++] = _activity[i][j];
            }
        }

        if (rhs._activity != nullptr) {
            for (int j = 0; j < rhs._monthly_activity_frequency[i]; j++) {
                new_activity[i][pos++] = rhs._activity[i][j];
            }
        }

        sort_transactions(new_activity[i], new_monthly_activity_frequency[i]);
    }

    if (_activity != nullptr) {
        for (int i = 0; i < 12; i++) {
            delete [] _activity[i];
        }

        delete [] _activity;
    }

    if (_monthly_activity_frequency != nullptr) {
        delete [] _monthly_activity_frequency;
    }

    _activity = new_activity;
    _monthly_activity_frequency = new_monthly_activity_frequency;

    return *this;
}

double Account::balance() {
    double result = 0;

    if (_activity == nullptr || _monthly_activity_frequency == nullptr) {
        return result;
    }

    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < _monthly_activity_frequency[i]; j++) {
            result = _activity[i][j] + result;
        }
    }

    return result;
}

double Account::balance(time_t end_date) {
    double result = 0;

    if (_activity == nullptr || _monthly_activity_frequency == nullptr) {
        return result;
    }

    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < _monthly_activity_frequency[i]; j++) {
            if (_activity[i][j] < end_date) {
                result = _activity[i][j] + result;
            }
        }
    }

    return result;
}

double Account::balance(time_t start_date, time_t end_date) {
    double result = 0;

    if (_activity == nullptr || _monthly_activity_frequency == nullptr) {
        return result;
    }

    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < _monthly_activity_frequency[i]; j++) {
            if (_activity[i][j] > start_date && _activity[i][j] < end_date) {
                result = _activity[i][j] + result;
            }
        }
    }

    return result;
}

std::ostream& operator<<(std::ostream& os, const Account& account) {
    if (account._activity == nullptr || account._monthly_activity_frequency == nullptr) {
        os << -1 << std::endl;
        return os;
    }

    os << account._id << std::endl;

    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < account._monthly_activity_frequency[i]; j++) {
            os << account._activity[i][j];
        }
    }

    return os;
}
