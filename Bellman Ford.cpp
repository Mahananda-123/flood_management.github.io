#include <iostream>
#include <vector>
#include <limits.h>

using namespace std;

struct Edge {
    int src, dest, weight;
};

class Graph {
public:
    int V, E;
    vector<Edge> edges;

    Graph(int V, int E) {
        this->V = V;
        this->E = E;
    }

    void addEdge(int src, int dest, int weight) {
        Edge edge = {src, dest, weight};
        edges.push_back(edge);
    }

    void bellmanFord(int start) {
        vector<int> dist(V, INT_MAX);
        dist[start] = 0;

        // Relax all edges V-1 times
        for (int i = 1; i <= V - 1; i++) {
            for (auto edge : edges) {
                if (dist[edge.src] != INT_MAX && dist[edge.src] + edge.weight < dist[edge.dest]) {
                    dist[edge.dest] = dist[edge.src] + edge.weight;
                }
            }
        }

        // Check for negative weight cycles
        for (auto edge : edges) {
            if (dist[edge.src] != INT_MAX && dist[edge.src] + edge.weight < dist[edge.dest]) {
                cout << "Graph contains negative weight cycle\n";
                return;
            }
        }

        // Print the distances
        cout << "Vertex   Distance from Source (" << start << ")\n";
        for (int i = 0; i < V; i++) {
            if (dist[i] == INT_MAX) {
                cout << i << "        INF\n";
            } else {
                cout << i << "        " << dist[i] << "\n";
            }
        }
    }
};

int main() {
    int V = 5; // Number of vertices
    int E = 8; // Number of edges

    Graph g(V, E);

    // Adding edges (src, dest, weight)
    g.addEdge(0, 1, -1);
    g.addEdge(0, 2, 4);
    g.addEdge(1, 2, 3);
    g.addEdge(1, 3, 2);
    g.addEdge(1, 4, 2);
    g.addEdge(3, 2, 5);
    g.addEdge(3, 1, 1);
    g.addEdge(4, 3, -3);

    // Run Bellman-Ford algorithm
    g.bellmanFord(0);

    return 0;
}
