#include "the5.h"

int main() {
    std::vector<int> solution;

    int n1 = 3, n2 = 9;
    int market[n1][n2] = {{ 10, -2, 5, 1, 7, 12, 54, 11, 2153 },
                { 5, -4, -1, 12, 213, 43, 34, 14, 12},
                {2, 14, 65, 123, 43, 234, 53, 32, -168}};

    int **market_p = new int*[n1];
    for (int i = 0; i < n1; i++) {
        market_p[i] = new int[n2];
        for (int j = 0; j < n2; j++) {
            market_p[i][j] = market[i][j];
        }
    }

    std::cout << BuyStocks(market_p, n1, n2, solution) << std::endl;
    for (int i = 0; i < n1; i++) {
        std::cout << solution[i] << " ";
    }
    std::cout << std::endl;
}
