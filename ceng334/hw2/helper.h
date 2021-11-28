#ifndef HW2_HELPER_H
#define HW2_HELPER_H

#define UNIT_TIME 100
#include <unistd.h>
#include <sys/time.h>
#include <limits.h>

// Calculate spent range for the drone when travelling the distance using the speed
int range_decrease(int distance, int speed);
// Sleep for the calculated travel time
void travel(int distance, int speed);
// Given the waiting duration of the drone in milliseconds, calculates the range gain for the drone and
// Returns the final range based on the calculation
int calculate_drone_charge(long long ms, int current_range, int max_range);
// Sleep for given milliseconds
void wait(long long ms);
// Gets the current time since epoch in milliseconds
long long timeInMilliseconds(void);

#endif //HW2_HELPER_H
