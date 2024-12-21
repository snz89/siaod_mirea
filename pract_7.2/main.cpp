#include <iostream>
#include <vector>
#include <climits>

using namespace std;

const int INF = INT_MAX;

vector<vector<int>> graphFromInput() {
    int n;
    cout << "Enter the number of vertices: ";
    cin >> n;

    vector<vector<int>> graph(n, vector<int>(n, 0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                cout << "Enter the weight of the edge from vertex " << i << " to vertex " << j << ": ";
                int weight;
                cin >> weight;
                if (weight < 0) {
                    weight = INF;
                    continue;
                }
                graph[i][j] = weight;
            }
        }
    }

    return graph;
}

vector<vector<int>> floydWarshall(const vector<vector<int>>& graph) {
    int numVertices = graph.size();

    vector<vector<int>> dist = graph;

    for (int k = 0; k < numVertices; ++k) {
        for (int i = 0; i < numVertices; ++i) {
            for (int j = 0; j < numVertices; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }
    
    return dist;
}

/**
 * Returns the distance of the shortest path from point to point using Floyd's method.
 * 
 * @note If there is no path, returns `2147483647`.
 * @note If non-existent points are passed as arguments, returns `-1`.
 * 
 * @param from the starting point
 * @param to the end point
 * @param graph adjacency matrix of the graph
 */
int getShortestRoute(int from, int to, const vector<vector<int>>& graph) {
    size_t n = graph.size();
    if (from >= n || to >= n || from < 0 || to < 0) {
        return -1;
    }
    auto dist = floydWarshall(graph);
    return dist[from][to];
}

/**
 * Formatted matrix output to a standard output stream.
 * If any value is equal to the maximum possible value for int,
 * it is replaced by the string ‘INF’.
 */
void printMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            if (val == INF) {
                cout << "INF\t";
            } else {
                cout << val << "\t";
            } 
        }
        cout << endl;
    }
}

int main()
{
    vector<vector<int>> exampleGraph = {
        {0, 2, 4, 2, INF, INF, INF, INF, INF, INF},
        {INF, 0, INF, INF, INF, 3, INF, INF, INF, INF},
        {INF, INF, 0, INF, INF, 6, INF, INF, INF, INF},
        {INF, INF, INF, 0, 5, 2, INF, INF, INF, INF},
        {INF, INF, INF, INF, 0, INF, 6, INF, 12, INF},
        {INF, INF, INF, INF, 1, 0, 8, 7, INF, INF},
        {INF, INF, INF, INF, INF, INF, 0, INF, INF, 4},
        {INF, INF, INF, INF, INF, INF, INF, 0, INF, 3},
        {INF, INF, INF, INF, INF, INF, INF, 6, 0, 11},
        {INF, INF, INF, INF, INF, INF, INF, INF, INF, 0}
    };

    vector<vector<int>> currentGraph = exampleGraph;

    cout << "1. Example graph\n";
    cout << "2. Graph from input\n";
    cout << "3. Floyd's algorithm matrix\n";
    cout << "4. Shortest route\n";
    cout << "5. Print graph\n";
    cout << "6. Help\n";
    cout << "0. Exit\n";

    while (true) {
        cout << "Enter the command: ";
        int command;
        cin >> command;

        switch (command) {
            case 1:
                currentGraph = exampleGraph;
                cout << "Example graph selected\n";
                break;
            case 2:
                currentGraph = graphFromInput();
                cout << "Graph from input selected\n";
                break;
            case 3:
                printMatrix(floydWarshall(currentGraph));
                cout << "Floyd's algorithm matrix printed\n";
                break;
            case 4: {
                cout << "Enter the starting point: ";
                int from;
                cin >> from;
                cout << "Enter the end point: ";
                int to;
                cin >> to;
                int result = getShortestRoute(from - 1, to - 1, currentGraph);
                if (result == -1) {
                    cout << "Invalid points\n";
                } else {
                    cout << "Shortest route: " << result << endl;
                }
                break;
            }
            case 5:
                printMatrix(currentGraph);
                cout << "Graph printed\n";
                break;
            case 6:
                cout << "1. Example graph\n";
                cout << "2. Graph from input\n";
                cout << "3. Floyd's algorithm matrix\n";
                cout << "4. Shortest route\n";
                cout << "5. Print graph\n";
                cout << "6. Help\n";
                cout << "0. Exit\n";
                break;
            case 0:
                cout << "Exiting the program\n";
                return 0;
            default:
                cout << "Invalid command\n";
                break;
        }
    }
}
