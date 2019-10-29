#include <stdio.h>
#include <math.h>

#define MIN_DIFF pow(10, -5)

double f(double L, double n, double p, double r) {
    double d = pow(r + 1, n);

    return (d * p) - (r * L * d) - p;
}

double f_prime(double L, double n, double p, double r) {
    double d1 = pow(r + 1, n - 1);
    double d2 = d1 * (r + 1);

    return (n * d1 * p) - (L * d2) - (L * r * n * d1);
}

int main() {
    int which_is_missing;
    double L, n, p, r, result, x;

    scanf("%d ", &which_is_missing);

    if (which_is_missing == 1) {
        scanf("%lf %lf %lf", &n, &p, &r);

        result = pow(r + 1, n);
        result = (result - 1) * p / (r * result);
    }
    else if (which_is_missing == 2) {
        scanf("%lf %lf %lf", &L, &p, &r);

        result = (p / r) / ((p / r) - L);
        result = log10(result) / log10(r + 1);
    }
    else if (which_is_missing == 3) {
        scanf("%lf %lf %lf", &L, &n, &r);

        result = pow(r + 1, n);
        result = result * L * r / (result - 1);
    }
    else if (which_is_missing == 4) {
        scanf("%lf %lf %lf", &L, &n, &p);

        result = 0.5;

        while (fabs(x = f(L, n, p, result)) >= MIN_DIFF) {
            result = result - (x / f_prime(L, n, p, result));
        }
    }

    printf("%.6f", result);

    return 0;
}