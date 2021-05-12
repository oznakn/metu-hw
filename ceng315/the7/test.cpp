#include "the7.h"

int main()
{
    int n, src, dest, **times_p, **costs_p;
    std::vector<std::pair<int,int>> goodies;
    std::vector<int> cost_path;


    goodies.erase(goodies.begin(), goodies.end());
    cost_path.erase(cost_path.begin(), cost_path.end());
    n = 4;
    src = 0;
    dest = 2;
    int times_1[4][4] = {{0, 2, 0, 1},
                     {2, 0, 1, 2},
                     {0, 1, 0, 3},
                     {1, 2, 3, 0}};

    int costs_1[4][4] = {{0, 10, 0, 5},
                     {10, 0, 2, 9},
                     {0, 2, 0, 7},
                     {5, 9, 7, 0}};


    times_p = new int*[n];
    costs_p = new int*[n];
    for (int i = 0; i < n; i++) {
        times_p[i] = new int[n];
        costs_p[i] = new int[n];

        for (int j = 0; j < n; j++) {
            times_p[i][j] = times_1[i][j];
            costs_p[i][j] = costs_1[i][j];
        }
    }

    Wubbalubbadubdub(times_p, costs_p, n, src, dest, goodies, cost_path);

    for (int i = 0; i < cost_path.size(); i++) {
        std::cout << cost_path[i] << " ";
    }
    std::cout << std::endl;

    goodies.erase(goodies.begin(), goodies.end());
    cost_path.erase(cost_path.begin(), cost_path.end());

    n = 5;
    src = 0;
    dest = 3;
    int times_2[5][5] = {{ 0, 1, 3, 0, 0},
                      {1, 0, 1, 1, 0},
                      {3, 1, 0, 0, 2},
                      {0, 1, 0, 0, 3},
                      {0 , 0, 2, 3, 0}};

    int costs_2[5][5] = {{0, 10, 5, 0, 0},
                        {10, 0, 2, 5, 0},
                        {5, 2, 0, 0, 3},
                        {0, 5, 0, 0, 2},
                        {0, 0, 3, 2, 0}};

    goodies.push_back(std::make_pair(2, 10));

    times_p = new int*[n];
    costs_p = new int*[n];
    for (int i = 0; i < n; i++) {
        times_p[i] = new int[n];
        costs_p[i] = new int[n];

        for (int j = 0; j < n; j++) {
            times_p[i][j] = times_2[i][j];
            costs_p[i][j] = costs_2[i][j];
        }
    }

    Wubbalubbadubdub(times_p, costs_p, n, src, dest, goodies, cost_path);

    for (int i = 0; i < cost_path.size(); i++) {
        std::cout << cost_path[i] << " ";
    }
    std::cout << std::endl;

    goodies.erase(goodies.begin(), goodies.end());
    cost_path.erase(cost_path.begin(), cost_path.end());

    n = 6;
    src = 0;
    dest = 5;
    int times_3[6][6] = {{0, 1, 2, 0, 0, 0},
                        {1, 0, 1, 3, 2, 0},
                        {2, 1, 0, 0, 3, 0},
                        {0, 3, 0, 0, 3, 4},
                        {0, 2, 3, 3, 0, 2},
                        {0, 0, 0, 4, 2, 0}};

    int costs_3[6][6] = {{ 0, 6, 4, 0, 0, 0},
                        {6, 0, 1, 7, 5, 0},
                        {4, 1, 0, 0, 2, 0},
                        {0, 7, 0, 0, 4, 3},
                        {0, 5, 2, 4, 0, 5},
                        {0, 0, 0, 3, 5, 0}};

    goodies.push_back(std::make_pair(2, 10));
    goodies.push_back(std::make_pair(3, 5));

    times_p = new int*[n];
    costs_p = new int*[n];
    for (int i = 0; i < n; i++) {
        times_p[i] = new int[n];
        costs_p[i] = new int[n];

        for (int j = 0; j < n; j++) {
            times_p[i][j] = times_3[i][j];
            costs_p[i][j] = costs_3[i][j];
        }
    }

    Wubbalubbadubdub(times_p, costs_p, n, src, dest, goodies, cost_path);

    for (int i = 0; i < cost_path.size(); i++) {
        std::cout << cost_path[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
