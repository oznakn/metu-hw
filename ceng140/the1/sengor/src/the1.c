#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "the1_read_input.h"
#include "the1_recursive.h"
#include "the1_iterative.h"
#include "the1_serve_cust.h"

#define MAX_CUST_REQUIREMENT 1000
#define MAX_SHIP_TYPE_COUNT 10
#define MAX_CUST_COUNT 10

int available_captain_count;
int ship_type_count;
int customer_count;
int ship_types[MAX_SHIP_TYPE_COUNT];
int customer_requirements[MAX_CUST_COUNT];


/*
	+ You can write any code you want in the main() function, this is your free area.
	+ You don't need to submit this file, since your codes will be evaluated with my own main() implementation. At this point,
	  you should understand that you don't have to consider an output format, returning correct results is just fine. Sample
	  output printing code is shared with you below.

*/
int main() {
	int min_required_ships[MAX_CUST_COUNT] = {0};
	int i;

	read_input();

	for (i = 0; i < customer_count; i++) {
		min_required_ships[i] = min_ships_required_recursive(ship_types, ship_type_count, customer_requirements[i]);

		printf("%d ships required for customer %d\n", min_required_ships[i], i);
	}

	printf("%d\n", serve_customers(min_required_ships, customer_count, available_captain_count));

	/* read_input(); */

	for (i = 0; i < customer_count; i++) {
		min_required_ships[i] = min_ships_required_iterative(ship_types, ship_type_count, customer_requirements[i]);

		printf("%d ships required for customer %d\n", min_required_ships[i], i);
	}

	printf("%d\n", serve_customers(min_required_ships, customer_count, available_captain_count));

	/*
	min_required_ships[0] = 5;
	min_required_ships[1] = 9;
	min_required_ships[2] = 2;
	min_required_ships[3] = 4;
	min_required_ships[4] = 7;

	printf("%d\n", serve_customers(min_required_ships, 5, 28) == 5);
	printf("%d\n", serve_customers(min_required_ships, 5, 27) == 5);
	printf("%d\n", serve_customers(min_required_ships, 5, 19) == 4);
	printf("%d\n", serve_customers(min_required_ships, 5, 18) == 4);
	printf("%d\n", serve_customers(min_required_ships, 5, 13) == 3);
	printf("%d\n", serve_customers(min_required_ships, 5, 11) == 3);
	printf("%d\n", serve_customers(min_required_ships, 5, 8)  == 2);
	printf("%d\n", serve_customers(min_required_ships, 5, 6)  == 2);
	printf("%d\n", serve_customers(min_required_ships, 5, 5)  == 1);
	printf("%d\n", serve_customers(min_required_ships, 5, 2)  == 1);
	printf("%d\n", serve_customers(min_required_ships, 5, 1)  == 0);
*/

    return 0;
}




