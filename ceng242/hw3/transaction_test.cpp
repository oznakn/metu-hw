#include "Transaction.h"
#include <iostream>
int main() {
    struct tm some_date = {0};
    some_date.tm_hour = 13;   some_date.tm_min = 12; some_date.tm_sec = 39;
    some_date.tm_year = 119; some_date.tm_mon = 4; some_date.tm_mday = 13;
    time_t some_date_time = mktime(&some_date);



    std::cout<<"Transaction Constructor"<<std::endl;
    Transaction ele1(1,some_date_time);
    Transaction ele2(2,some_date_time);
    some_date_time += 1000;
    Transaction ele3(3,some_date_time);
    std::cout<<"Stream Overload"<<std::endl;
    std::cout<<ele1<<std::endl;
    std::cout<<ele2<<std::endl;
    std::cout<<ele3<<std::endl;
    std::cout<<"Plus Overload"<<std::endl;
    std::cout<<ele1+ele2<<std::endl;

    std::cout<<"Comparison"<<std::endl;
    if (ele1<ele3)
        std::cout<<"Correct"<<std::endl;
    if (ele3>ele1)
        std::cout<<"Correct"<<std::endl;

    return 0;
}
