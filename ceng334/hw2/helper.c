#include "helper.h"

int range_decrease(int distance, int speed) {
    return distance/speed;
}

void travel(int distance, int speed) {
    long long time = distance * UNIT_TIME / speed;
    wait(time);
}


int calculate_drone_charge(long long ms, int current_range, int max_range) {
    int charged_time_in_int = 0;
    if ( ms > INT_MAX )
        charged_time_in_int = INT_MAX;
    else
        charged_time_in_int = (int)ms;
    int charged_range = charged_time_in_int/UNIT_TIME;
    int total_range = current_range + charged_range;
    if ( total_range > max_range )
        return max_range;
    return total_range;
}

void wait(long long ms) {
    long long time = ms * 1000;
    long long seconds = time/1000000;
    long long microseconds = time%1000000;
    if ( seconds > 0 )
        sleep(seconds);
    if ( microseconds > 0 )
        usleep(microseconds);
}

long long timeInMilliseconds(void) {
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}
