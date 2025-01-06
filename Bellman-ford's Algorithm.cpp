#include <iostream>
#include <vector>
#include <climits>

using namespace std;

struct Edge {
    int src, dest, weight;
};

void bellmanFord(const vector<Edge>& edges, int V, int E, int start) {
    vector<int> dist(V, INT_MAX);
    dist[start] = 0;

    for (int i = 0; i < V - 1; i++) {
        for (const auto& edge : edges) {
            if (dist[edge.src] != INT_MAX && dist[edge.src] + edge.weight < dist[edge.dest]) {
                dist[edge.dest] = dist[edge.src] + edge.weight;
            }
        }
    }

    for (const auto& edge : edges) {
        if (dist[edge.src] != INT_MAX && dist[edge.src] + edge.weight < dist[edge.dest]) {
            cout << "Graph contains negative weight cycle" << endl;
            return;
        }
    }

    cout << "Shortest distances from source " << start << ":" << endl;
    for (int i = 0; i < V; i++) {
        if (dist[i] == INT_MAX)
            cout << "INF ";
        else
            cout << dist[i] << " ";
    }
    cout << endl;
}

int main() {
    int V = 5;
    int E = 8;
    vector<Edge> edges = {
        {0, 1, -1}, {0, 2, 4}, {1, 2, 3}, {1, 3, 2},
        {1, 4, 2}, {3, 1, 1}, {3, 2, 5}, {4, 3, -3}
    };

    int start = 0;
    bellmanFord(edges, V, E, start);

    return 0;
}
