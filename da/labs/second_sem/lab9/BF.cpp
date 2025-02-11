#include <iostream>
#include <vector>
#include <limits>
#include <chrono>

using namespace std;

const long long INF = numeric_limits<long long>::max();

class Graph {
private:
    struct Edge {
        int from, to;
        long long weight;
    };

    int verticesCount;
    vector<Edge> edges;

public:
    Graph(int n) : verticesCount(n) {}

    void addEdge(int from, int to, long long weight) {
        edges.push_back({from - 1, to - 1, weight}); // Конвертация к 0-индексации
    }

    long long findShortestPath(int start, int end) {
        vector<long long> distances(verticesCount, INF);
        distances[start - 1] = 0;

        for (int i = 0; i < verticesCount - 1; ++i) {
            bool hasChanges = false;

            for (const auto& edge : edges) {
                if (distances[edge.from] != INF && distances[edge.from] + edge.weight < distances[edge.to]) {
                    distances[edge.to] = distances[edge.from] + edge.weight;
                    hasChanges = true;
                }
            }

            if (!hasChanges) break;
        }

        return distances[end - 1];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, start, end;
    cin >> n >> m >> start >> end;

    Graph graph(n);

    for (int i = 0; i < m; ++i) {
        int from, to;
        long long weight;
        cin >> from >> to >> weight;
        graph.addEdge(from, to, weight);
    }

    long long shortestPath = graph.findShortestPath(start, end);

    if (shortestPath == INF) {
        cout << "No solution" << endl;
    } else {
        cout << shortestPath << endl;
    }

    return 0;
}
