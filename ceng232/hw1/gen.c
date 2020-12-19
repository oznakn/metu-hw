#include <stdio.h>

void print_num(int n) {
	switch (n) {
		case 0: {
			printf("00");
			break;
		}

		case 1: {
			printf("01");
			break;
		}

		case 2: {
			printf("10");
			break;
		}

		case 3: {
			printf("11");
			break;
		}
	}
}

int main() {
	for (int a = 0; a < 4; a++) {
		for (int b = 0; b < 4; b++) {
			print_num(a);
			printf(" ");
			print_num(b);
			printf(" ");

			if (a + b > 2) {
				printf("1");
			} else {
				printf("0");
			}
			printf(" ");

			if ((b == 0 && a == 1) || (b != 0 && (a+b) % b == 1)) {
				printf("1");
			} else {
				printf("0");
			}
			printf(" ");

			if ((a << b) > (b << a)) {
				printf("1");
			} else {
				printf("0");
			}

			printf("\n");
		}
	}

	return 0;
}