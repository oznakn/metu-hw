#include "the8.h"

const int INF = 100;

int selectMin(std::vector<int> & queue, int distances[], int V) {
    int selected = 0;

    for (int i = 0; i < queue.size(); i++) {
        if (distances[queue[selected]] > distances[queue[i]]) {
            selected = i;
        }
    }

    int result = queue[selected];

    queue.erase(queue.begin() + selected);

    return result;
}

std::vector<int> dijkstra(int **& graph, int V, int start, int end) {
    std::vector<int> queue;
    int distances[V];
    int parents[V];

    for (int i = 0; i < V; i++) {
        queue.push_back(i);
        distances[i] = INF;
        parents[i] = -1;
    }

    distances[start] = 0;

    while (!queue.empty()) {
        int head = selectMin(queue, distances, V);

        for (int i = 0; i < V; i++) {
            if (head != i && graph[head][i] < INF) {
                int newDistance = distances[head] + graph[head][i];

                if (distances[i] == INF || distances[i] >= newDistance) {
                    distances[i] = newDistance;
                    parents[i] = head;
                }
            }
        }
    }

    std::vector<int> path;

    int head = end;

    while (head != -1 && head != start) {
        path.insert(path.begin(), head);
        head = parents[head];
    }

    path.insert(path.begin(), start);

    return path;
}

std::pair<int, int> lcs(std::vector<int> & path1, std::vector<int> & path2) {
    int i, j;
    int m = path1.size();
    int n = path2.size();

    int dp[m + 1][n + 1];

    for (i = 0; i < m + 1; i++) {
        for (j = 0; j < n + 1; j++) {
            if (i == 0 || j == 0) {
                dp[i][j] = 0;
            } else if (path1[i - 1] == path2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    std::pair<int, int> result;
    result.first = -1;
    result.second = -1;

    if (dp[m][n] <= 1) {
        return result;
    }

    int index = dp[m][n];

    i = m;
    j = n;

    while (i > 0 && j > 0) {
        if (path1[i - 1] == path2[j - 1]) {
            index--;
            i--;
            j--;

            if (index == dp[m][n] - 1) {
                result.second = path1[i];
            } else if (index == 0) {
                result.first = path1[i];
            }
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        } else{
            j--;
        }
    }

    return result;
}

int minCost(int**& graph, int V, int s1, int s2, int d1, int d2, int A, int B) {
    int distances[V][V];

    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            distances[i][j] = graph[i][j];
        }
    }

    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (distances[i][k] >= INF || distances[k][j] >= INF) {
                    distances[i][j] = std::min(distances[i][j], INF);
                } else {
                    distances[i][j] = std::min(distances[i][j], distances[i][k] + distances[k][j]);
                }
            }
        }
    }

    std::vector<int> path1 = dijkstra(graph, V, s1, d1);
    std::vector<int> path2 = dijkstra(graph, V, s2, d2);

    std::pair<int, int> commonPath = lcs(path1, path2);

    if (commonPath.first == -1 || commonPath.second == -1) {
        return A * (distances[s1][d1] + distances[s2][d2]);
    }

    return A * (distances[s1][commonPath.first] + distances[commonPath.second][d1]) +
            A * (distances[s2][commonPath.first] + distances[commonPath.second][d2]) +
            distances[commonPath.first][commonPath.second] * B;
}
