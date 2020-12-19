#include <iostream>
#include <ctime>

int main() {
    const int month_day_counts[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    std::cout << "{";

    for (int i = 0; i < 12; i++) {
        struct tm time = {0};
        time.tm_year = 2019 - 1900;
        time.tm_mon = i;

        time.tm_mday = 1;
        time.tm_hour = 0;
        time.tm_min = 0;
        time.tm_sec = 0;
        std::cout << mktime(&time) << ", ";


        time.tm_mday = month_day_counts[i];
        time.tm_hour = 23;
        time.tm_min = 59;
        time.tm_sec = 59;
        std::cout << mktime(&time) << ", ";
    }

    std::cout << "}" << std::endl;

    return 0;
}
