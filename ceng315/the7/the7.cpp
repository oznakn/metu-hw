#include "the7.h"

// do not add extra libraries here

typedef struct {
    int start;
    int end;
    int weight;
    int goodie;
} Edge;

void Wubbalubbadubdub(int**& times, int**& costs, int n, int src, int dest, std::vector<std::pair<int,int>>& goodies, std::vector<int>& cost_path) {
    int distances[n];
    int goodie_nodes[n];
    std::vector<int> parents[n];
    std::vector<Edge> virtual_edges;

    for (int i = 0; i < n; i++) {
        distances[i] = INT_MAX;
        goodie_nodes[i] = 0;

        parents[i].push_back(-1);
    }

    for (int i = 0; i < goodies.size(); i++) {
        goodie_nodes[goodies[i].first] = goodies[i].second;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (times[i][j] != 0 && costs[i][j] != 0) {
                Edge edge;
                edge.start = i;
                edge.end = j;
                edge.weight = times[i][j] * costs[i][j];
                edge.goodie = goodie_nodes[j];

                virtual_edges.push_back(edge);
            }
        }
    }

    distances[src] = 0;

    for (int i = 0; i < n; i++) {
        std::vector<int> used_nodes;

        for (int j = 0; j < virtual_edges.size(); j++) {
            Edge edge = virtual_edges[j];

            if (distances[edge.start] != INT_MAX) {
                int new_distance = distances[edge.start] + edge.weight - edge.goodie;

                if (distances[edge.end] > new_distance) {
                    distances[edge.end] = new_distance;
                    used_nodes.push_back(j);

                    parents[edge.end].push_back(edge.start);
                }
            }
        }

        for (int j = 0; j < used_nodes.size(); j++) {
            virtual_edges[j].goodie = 0;
        }
    }

    int head = dest;

    while (head != -1) {
        cost_path.insert(cost_path.begin(), head);

        int temp = parents[head].back();
        parents[head].pop_back();
        head = temp;
    }

    cost_path.insert(cost_path.begin(), distances[dest]);
}
