#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Edge {
public:
    int src, dest, weight;
    Edge(int s, int d, int w) : src(s), dest(d), weight(w) {}
};

bool compare(Edge e1, Edge e2) {
    return e1.weight < e2.weight;
}

int findParent(int parent[], int i) {
    if (parent[i] == i)
        return i;
    return findParent(parent, parent[i]);
}

void unionSets(int parent[], int rank[], int x, int y) {
    int xroot = findParent(parent, x);
    int yroot = findParent(parent, y);

    if (xroot != yroot) {
        if (rank[xroot] < rank[yroot])
            parent[xroot] = yroot;
        else if (rank[xroot] > rank[yroot])
            parent[yroot] = xroot;
        else {
            parent[yroot] = xroot;
            rank[xroot]++;
        }
    }
}

void kruskalMST(vector<Edge>& edges, int V) {
    sort(edges.begin(), edges.end(), compare);
    int parent[V], rank[V];
    for (int i = 0; i < V; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    vector<Edge> result;

    for (Edge& e : edges) {
        int x = findParent(parent, e.src);
        int y = findParent(parent, e.dest);

        if (x != y) {
            result.push_back(e);
            unionSets(parent, rank, x, y);
        }
    }

    cout << "Edge \tWeight\n";
    for (Edge& e : result) {
        cout << e.src << " - " << e.dest << " \t" << e.weight << endl;
    }
}

int main() {
    int V = 4;
    vector<Edge> edges = { Edge(0, 1, 10), Edge(0, 2, 6), Edge(0, 3, 5),
                            Edge(1, 3, 15), Edge(2, 3, 4) };

    kruskalMST(edges, V);
    return 0;
}
