#include "Transaction.h"
#include "Account.h"
#include "Bank.h"
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

void helper(std::vector<time_t>&  time_arr, Transaction** tr, int* val, Transaction** tr2, int* val2) {

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
        } else if(counter<180) {
            if(counter%5 == 0) {
                val2[(counter-120)/5] = 5;
                tr2[(counter-120)/5] = new Transaction[5];
                tr2[(counter-120)/5][counter%5] = Transaction(std::stoi(line), time_arr[((counter-120)/5)*5+(counter%5)]);
            } else {
                tr2[(counter-120)/5][counter%5] = Transaction(std::stoi(line), time_arr[((counter-120)/5)*5+(counter%5)]);
            }
        }
        counter++;

    }
}
int main() {


    Transaction** new_trans = new Transaction*[12];
    int* values = new int[12];
    Transaction** new_trans2 = new Transaction*[12];
    int* values2 = new int[12];
    std::vector<time_t> time_arr(60);

    helper(time_arr, new_trans, values, new_trans2, values2);
    std::cout<<"Create resources for Bank"<<std::endl;
    Account my_acc1(1, new_trans, values);
    Account my_acc2(2, new_trans2, values2);
    Account my_acc3(3, new_trans2, values2);
    Account my_acc4(4, new_trans2, values2);
    Account my_acc5(5, new_trans2, values2);
    Account acc1(my_acc1, time_arr[0], time_arr[15]);
    Account acc2(my_acc2, time_arr[0], time_arr[15]);
    Account acc3(my_acc3, time_arr[15], time_arr[30]);
    Account acc4(my_acc4, time_arr[30], time_arr[45]);
    Account acc5(my_acc5, time_arr[45], time_arr[59]);
    std::cout<<"ACC 1"<<std::endl;
    std::cout<<acc1;
    std::cout<<"ACC 2"<<std::endl;
    std::cout<<acc2;
    std::cout<<"ACC 3"<<std::endl;
    std::cout<<acc3;
    std::cout<<"ACC 4"<<std::endl;
    std::cout<<acc4;
    std::cout<<"ACC 5"<<std::endl;
    std::cout<<acc5;
    Account * acc_arr1 = new Account[2];
    Account * acc_arr2 = new Account[2];
    std::cout<<"Assign Resources"<<std::endl;
    acc_arr1[0] = acc1;
    acc_arr1[1] = acc2;
    acc_arr2[0] = acc3;
    acc_arr2[1] = acc4;
    std::cout<<"Constructor"<<std::endl;
    Bank my_bank1("bananas", acc_arr1, 2);
    Bank my_bank2("monkeys", acc_arr2, 2);
    std::cout<<"Stream overlaod"<<std::endl;
    std::cout<<my_bank1;
    std::cout<<my_bank2;
    std::cout<<"Index overload"<<std::endl;
    std::cout<<my_bank1[1];
    std::cout<<"Sum and equal overload with Bank"<<std::endl;
    my_bank1+=my_bank2;
    std::cout<<my_bank1;
    std::cout<<my_bank1[3];
    std::cout<<"Sum and equal overload with Account"<<std::endl;
    my_bank2+= acc5;
    std::cout<<my_bank2;
    std::cout<<my_bank2[5];

    for(int i = 0;i<12;i++) {
        delete[] new_trans[i];
        delete[] new_trans2[i];
    }
    delete[] acc_arr1;
    delete[] acc_arr2;
    delete[] new_trans;
    delete[] new_trans2;
    delete[] values;
    delete[] values2;

    return 0;
}
