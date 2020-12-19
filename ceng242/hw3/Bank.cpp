#include "Bank.h"

Bank::Bank() {
    _bank_name = "not_defined";
    _user_count = 0;
    _users = nullptr;
}

Bank::Bank(std::string bank_name, Account* const users, int user_count) {
    _bank_name = bank_name;
    _user_count = user_count;

    _users = new Account[user_count];
    for (int i = 0; i < user_count; i++) {
        _users[i] = users[i];
    }
}

Bank::~Bank() {
    if (_users != nullptr) {
        delete [] _users;
    }
}

Bank::Bank(const Bank& rhs) {
    _bank_name = rhs._bank_name;
    _user_count = rhs._user_count;

    _users = new Account[rhs._user_count];
    for (int i = 0; i < rhs._user_count; i++) {
        _users[i] = rhs._users[i];
    }
}

Bank& Bank::operator+=(const Bank& rhs) {
    int new_user_count = _user_count;

    for (int j = 0; j < rhs._user_count; j++) {
        bool found = false;

        for (int i = 0; i < _user_count; i++) {
            if (rhs._users[j] == _users[i]) {
                found = true;
                break;
            }
        }

        if (!found) {
            new_user_count++;
        }
    }

    Account *new_users = new Account[new_user_count];
    int pos = 0;

    for (int i = 0; i < _user_count; i++) {
        new_users[pos] = _users[i];

        for (int j = 0; j < rhs._user_count; j++) {
            if (_users[i] == rhs._users[j]) {
                new_users[pos] += rhs._users[j];

                break;
            }
        }

        pos++;
    }

    for (int j = 0; j < rhs._user_count; j++) {
        bool found = false;

        for (int i = 0; i < _user_count; i++) {
            if (rhs._users[j] == _users[i]) {
                found = true;
                break;
            }
        }

        if (!found) {
            new_users[pos] = rhs._users[j];
            pos++;
        }
    }

    delete [] _users;

    _user_count = new_user_count;
    _users = new_users;

    return *this;
}

Bank& Bank::operator+=(const Account& new_acc) {
    bool found = false;

    for (int i = 0; i < _user_count; i++) {
        if (_users[i] == new_acc) {
            found = true;

            _users[i] += new_acc;

            break;
        }
    }

    if (!found) {
        Account *new_users = new Account[_user_count + 1];

        for (int i = 0; i < _user_count; i++) {
            new_users[i] = _users[i];
        }

        new_users[_user_count] = new_acc;

        delete [] _users;

        _user_count++;
        _users = new_users;
    }

    return *this;
}

Account& Bank::operator[](uint account_id) {
    for (int i = 0; i < _user_count; i++) {
        if (_users[i] == account_id) {
            return _users[i];
        }
    }

    return _users[0];
}

std::ostream& operator<<(std::ostream& os, const Bank& bank) {
    const int time_limits[] = {1546290000, 1548968399, 1548968400, 1551387599, 1551387600, 1554065999, 1554066000, 1556657999, 1556658000, 1559336399, 1559336400, 1561928399, 1561928400, 1564606799, 1564606800, 1567285199, 1567285200, 1569877199, 1569877200, 1572555599, 1572555600, 1575147599, 1575147600, 1577825999};

    double total_balance = 0;
    int loan_user_count = bank._user_count;

    for (int i = 0; i < bank._user_count; i++) {
        total_balance += bank._users[i].balance();

        double last_balance = 0;

        for (int m = 0; m < 12; m++) {
            double balance = bank._users[i].balance(time_limits[2*m], time_limits[(2*m) + 1]);

            if (last_balance < 0 && balance < 0) {
                loan_user_count--;
                break;
            }

            last_balance = balance;
        }
    }

    os << bank._bank_name << "\t" << loan_user_count << "\t" << total_balance << std::endl;

    return os;
}
