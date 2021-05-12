#include "the6.h"

// do not add extra libraries here

bool is_cyclic_helper(int**& graph, int n, int start, bool *visited, bool *explored) {
    visited[start] = true;
    explored[start] = true;

    for (int i = 0; i < n; i++) {
        if (graph[start][i] == 1) {
            if (!visited[i] && is_cyclic_helper(graph, n, i, visited, explored)) {
                return true;
            } else if (visited[i] && explored[i]) {
                return true;
            }
        }
    }

    explored[start] = false;

    return false;
}

bool is_cyclic(int**& graph, int n) {
    bool visited[n] = { false };
    bool explored[n] = { false };

    for (int i = 0; i < n; i++) {
        if (!visited[i] && is_cyclic_helper(graph, n, i, visited, explored)) {
            return true;
        }
    }

    return false;
}

int** get_transpose(int**& graph, int n) {
    int **transpose_graph = new int*[n];

    for (int i = 0; i < n; i++) {
        transpose_graph[i] = new int[n];

        for (int j = 0; j < n; j++) {
            transpose_graph[i][j] = 0;
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (graph[i][j] == 1) {
                transpose_graph[j][i] = 1;
            }
        }
    }

    return transpose_graph;
}

void find_reachable_nodes(int**& graph, int n, int start, bool *visited, std::stack<int>& result) {
    visited[start] = true;

    for (int i = 0; i < n; i++) {
        if (graph[start][i] == 1 && !visited[i]) {
            find_reachable_nodes(graph, n, i, visited, result);
        }
    }

    result.push(start);
}

void find_topsort(int**& graph, int n, std::stack<int>& result) {
    bool visited[n] = { false };

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            find_reachable_nodes(graph, n, i, visited, result);
        }
    }
}

void find_scc(int**& graph, int n, std::vector<std::vector<int> >& scc) {
    std::stack<int> result;

    find_topsort(graph, n, result);

    int **transpose_graph = get_transpose(graph, n);

    bool visited[n] = { false };

    while (!result.empty()) {
        int start = result.top();
        result.pop();

        if (!visited[start]) {
            std::stack<int> component;
            find_reachable_nodes(transpose_graph, n, start, visited, component);

            std::vector<int> vec;

            while (!component.empty()) {
                vec.push_back(component.top());
                component.pop();
            }

            scc.push_back(vec);
        }
    }

    for (int i = 0; i < n; i++) {
        delete[] transpose_graph[i];
    }
    delete[] transpose_graph;
}


void SCC_TopSort(int**& graph, int n, std::vector<std::vector<int> >& scc, std::vector<int>& topSort) {
    if (is_cyclic(graph, n)) {
        find_scc(graph, n, scc);
    } else {
        std::stack<int> result;

        find_topsort(graph, n, result);

        while (!result.empty()) {
            topSort.push_back(result.top());
            result.pop();
        }
    }
}
