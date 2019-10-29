#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "the1_read_input.h"

#define MAX_CUST_REQUIREMENT 1000
#define MAX_SHIP_TYPE_COUNT 10
#define MAX_CUST_COUNT 10

extern int available_captain_count;
extern int ship_type_count;
extern int customer_count;
extern int ship_types[MAX_SHIP_TYPE_COUNT];
extern int customer_requirements[MAX_CUST_COUNT];

/*
	+ 'extern' keyword allows you to use variables defined by the program and reads values from the memory.
	With 'extern', you are saying that this variable is defined in somewhere and defined before by the program.
	This means that you can use 'scanf()' function for filling your primary variables just like you did in a single 'main()' function.
	You should NOT write 'extern' at the beginning of variables in read_input() function. Lines [10-14] are enough.

	+ When return type is 'void', you don't return anything.
	Just for your information, if you really want to return and halt function execution at some point, use "return;".
	However, this is not necessary for read_input() function of course.
*/
void read_input() {
	int i;

	scanf("%d\n", &available_captain_count);
	scanf("%d\n", &ship_type_count);

	for (i = 0; i < ship_type_count; i++) {
		scanf("%d ", &ship_types[i]);
	}

	scanf("%d\n", &customer_count);

	for (i = 0; i < customer_count; i++) {
		scanf("%d ", &customer_requirements[i]);
	}
}