#include <iostream>
#include <limits.h>

using namespace std;

const int n = 5;

int minDistance(int dist[], bool sptSet[]) {
    int min = INT_MAX, min_index;
    for (int v = 0; v < n; v++)
        if (!sptSet[v] && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}

void printSolution(int dist[]) {
    cout << "Vertex \t Distance from Source" << endl;
    for (int i = 0; i < n; i++)
        cout << i << " \t\t " << dist[i] << endl;
}

void dijkstra(int graph[n][n], int src) {
    int dist[n];
    bool sptSet[n];
    for (int i = 0; i < n; i++)
        dist[i] = INT_MAX, sptSet[i] = false;
    dist[src] = 0;
    for (int count = 0; count < n - 1; count++) {
        int u = minDistance(dist, sptSet);
        sptSet[u] = true;
        for (int v = 0; v < n; v++) {
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
        }
    }
    printSolution(dist);
}

int main() {
    int graph[n][n] = { {0, 10, 0, 0, 5},
                        {10, 0, 1, 0, 2},
                        {0, 1, 0, 4, 0},
                        {0, 0, 4, 0, 3},
                        {5, 2, 0, 3, 0} };
    int src = 0;
    dijkstra(graph, src);
    return 0;
}
