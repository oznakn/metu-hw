#include "Transaction.h"
#include "Account.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <vector>
std::vector<std::string> split(const std::string& str,
              char delim = ' ')
{
    std::vector <std::string> cont;
    std::size_t current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos) {
        cont.push_back(str.substr(previous, current - previous));
        previous = current + 1;
        current = str.find(delim, previous);
    }
    cont.push_back(str.substr(previous, current - previous));
    return cont;
}

void helper(std::vector<time_t>&  time_arr, Transaction** tr, int* val) {

    std::ifstream infile("example.txt");
    std::string line;
    int counter = 0;
    while (std::getline(infile, line))
    {
        if(counter<60) {

            char delim = ' ';
            std::vector<std::string> arr = split(line, delim);

            struct tm temp = {0};

            temp.tm_year = std::stoi(arr[0]); temp.tm_mon = std::stoi(arr[1]); temp.tm_mday = std::stoi(arr[2]);
            temp.tm_hour = std::stoi(arr[3]); temp.tm_min = std::stoi(arr[4]); temp.tm_sec = std::stoi(arr[5]);
            time_arr[counter] = mktime(&temp);


        } else if (counter<120) {
            if(counter%5 == 0) {
                val[(counter-60)/5] = 5;
                tr[(counter-60)/5] = new Transaction[5];
                tr[(counter-60)/5][counter%5] = Transaction(std::stoi(line), time_arr[((counter-60)/5)*5+(counter%5)]);
            } else {
                tr[(counter-60)/5][counter%5] = Transaction(std::stoi(line), time_arr[((counter-60)/5)*5+(counter%5)]);
            }
        }
        counter++;

    }
}

int main() {
    std::vector<time_t> time_arr(60);

    Account acc1;

    Transaction** new_trans = new Transaction*[12];
    int* values = new int[12];
    helper(time_arr, new_trans, values);

    std::cout<<"Constructor"<<std::endl;
    Account my_acc(1, new_trans, values);
    std::cout<<"Copy constructor with given date"<<std::endl;
    Account my_first(my_acc, time_arr[0], time_arr[11]);
    Account my_sec(my_acc,time_arr[10], time_arr[20]);

    std::cout<<"Stream Overload"<<std::endl;
    std::cout<<my_acc<<std::endl;

    std::cout<<"Balance calculation without a Date"<<std::endl;
    std::cout<<my_acc.balance( )<<std::endl;

    std::cout<<asctime(localtime(&time_arr[0]))<<std::endl;
    std::cout<<asctime(localtime(&time_arr[10]))<<std::endl;
    std::cout<<"Balance calculation with only end Date"<<std::endl;
    std::cout<<my_acc.balance( time_arr[10])<<std::endl;

    std::cout<<"Balance calculation with both start and end date"<<std::endl;
    std::cout<<my_acc.balance( time_arr[0],time_arr[10])<<std::endl;

    std::cout<<"Sum and equal Overload"<<std::endl;
    my_first+=my_sec;
    std::cout<<my_first;

    std::cout<<"Move constructor"<<std::endl;
    Account move_const_test(std::move(my_sec));
    std::cout<<move_const_test<<std::endl;
    std::cout<<"after move"<<std::endl;
    std::cout<<my_sec<<std::endl;

    std::cout<<"Move assignment"<<std::endl;
    Account move_assign_test = std::move(move_const_test);
    std::cout<<move_assign_test<<std::endl;
    std::cout<<"after move"<<std::endl;
    std::cout<<move_const_test<<std::endl;

    for(int i = 0;i<12;i++) {
        delete[] new_trans[i];
    }

    delete[] new_trans;
    delete[] values;

    return 0;
}
