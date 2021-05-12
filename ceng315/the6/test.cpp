#include "the6.h"

int main() {
    int n = 7;
    int **p;

    std::vector<std::vector<int> > scc1;
    std::vector<int> topSort1;
    int test1[7][7] = { { 0, 1, 0, 1, 0, 0, 0 },
                        { 0, 0, 1, 0, 0, 0, 0 },
                        { 1, 0, 0, 0, 0, 0, 0 },
                        { 0, 0, 0, 0, 1, 0, 0 },
                        { 0, 0, 0, 0, 0, 1, 0 },
                        { 0, 0, 0, 0, 0, 0, 1 },
                        { 0, 0, 0, 0, 1, 0, 0 } };
    p = new int*[n];
    for (int i = 0; i < n; i++) {
        p[i] = new int[n];
        for (int j = 0; j < n; j++) {
            p[i][j] = test1[i][j];
        }
    }
    SCC_TopSort(p, n, scc1, topSort1);

    std::cout << "SCC" << std::endl;
    for (int i = 0; i < scc1.size(); i++) {
        for (int j = 0; j < scc1[i].size(); j++) {
            std::cout << scc1[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::vector<std::vector<int> > scc2;
    std::vector<int> topSort2;
    int test2[7][7] = { { 0, 0, 0, 0, 0, 0, 0 },
                        { 1, 0, 0, 0, 0, 0, 0 },
                        { 1, 0, 0, 0, 0, 0, 0 },
                        { 0, 0, 0, 0, 0, 0, 0 },
                        { 0, 1, 0, 1, 0, 0, 0 },
                        { 0, 0, 1, 1, 0, 0, 0 },
                        { 0, 0, 0, 0, 1, 1, 0 } };
    p = new int*[n];
    for (int i = 0; i < n; i++) {
        p[i] = new int[n];
        for (int j = 0; j < n; j++) {
            p[i][j] = test2[i][j];
        }
    }
    SCC_TopSort(p, n, scc2, topSort2);

    std::cout << "TopSort" << std::endl;
    for (int i = 0; i < topSort2.size(); i++) {
        std::cout << topSort2[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
