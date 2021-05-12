#include "the5.h"

int BuyStocks (int**& market, int n1, int n2, std::vector<int>& solution)
{
    int dp[n1][n2];

    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            dp[i][j] = INT_MIN;
        }
    }

    for (int i = 0; i < n1; i++) {
        for (int j = i; j < n2; j++) {
            if (i == 0) {
                if (j == 0) {
                    dp[i][j] = market[i][j];
                } else {
                    dp[i][j] = std::max(market[i][j], dp[i][j - 1]);
                }
            } else {
                dp[i][j] = std::max(dp[i - 1][j - 1] + market[i][j], dp[i][j - 1]);
            }
        }
    }

    int i = n1 - 1;
    for (int j = n2 - 1; j >= 0 && i >= 0; j--) {
        if (dp[i][j] != dp[i][j - 1]) {
            solution.insert(solution.begin(), j);
            i--;
        }
    }

    return dp[n1 - 1][n2 - 1];
}
