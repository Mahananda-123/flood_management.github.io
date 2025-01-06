#include <iostream>
#include <vector>

using namespace std;

void warshallAlgorithm(vector<vector<int>>& adj, int V) {
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (adj[i][j] == 0 && adj[i][k] == 1 && adj[k][j] == 1) {
                    adj[i][j] = 1;
                }
            }
        }
    }

    cout << "Reachability matrix:" << endl;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            cout << adj[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int V = 4;
    vector<vector<int>> adj = {
        {0, 1, 0, 1},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
        {0, 0, 0, 0}
    };

    warshallAlgorithm(adj, V);

    return 0;
}
