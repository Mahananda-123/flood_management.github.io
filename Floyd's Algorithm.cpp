#include <iostream>
#include <vector>
#include <climits>

using namespace std;

#define INF INT_MAX

void floydWarshall(vector<vector<int>>& dist, int V) {
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    cout << "Shortest distances between every pair of vertices:" << endl;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF)
                cout << "INF ";
            else
                cout << dist[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int V = 4;
    vector<vector<int>> dist(V, vector<int>(V, INF));

    dist[0][0] = 0; dist[0][1] = 3; dist[0][2] = INF; dist[0][3] = 7;
    dist[1][0] = 3; dist[1][1] = 0; dist[1][2] = 2; dist[1][3] = INF;
    dist[2][0] = INF; dist[2][1] = 2; dist[2][2] = 0; dist[2][3] = 1;
    dist[3][0] = 7; dist[3][1] = INF; dist[3][2] = 1; dist[3][3] = 0;

    floydWarshall(dist, V);

    return 0;
}
