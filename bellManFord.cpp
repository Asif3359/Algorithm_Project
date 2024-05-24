#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Structure to represent a weighted edge in the graph
struct bellEdges
{
    int src, dest, weight;
};

// Class to represent a graph for Bellman-Ford algorithm
class BellmanFordGraph
{
public:
    int V, E;
    vector<bellEdges> edges;

    BellmanFordGraph(int V, int E)
    {
        this->V = V;
        this->E = E;
        edges.resize(E);
    }

    void addEdge(int u, int v, int w, int i)
    {
        edges[i] = {u, v, w};
    }

    void bellmanFord(int src);
};

void BellmanFordGraph::bellmanFord(int src)
{
    vector<int> dist(V, INT_MAX);
    dist[src] = 0;

    auto start = high_resolution_clock::now(); // Start measuring time

    for (int i = 1; i <= V - 1; ++i)
    {
        for (int j = 0; j < E; ++j)
        {
            int u = edges[j].src;
            int v = edges[j].dest;
            int weight = edges[j].weight;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
            }
        }
    }

    for (int j = 0; j < E; ++j)
    {
        int u = edges[j].src;
        int v = edges[j].dest;
        int weight = edges[j].weight;
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
        {
            cout << "Graph contains negative weight cycle" << endl;
            return;
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start).count();

    cout << "Time taken by Bellman-Ford algorithm: " << duration << " microseconds" << endl;

    cout << "Shortest distances from location " << src << ":" << endl;
    for (int i = 0; i < V; ++i)
    {
        if (dist[i] == INT_MAX)
        {
            cout << "Location " << i << " is unreachable" << endl;
        }
        else
        {
            cout << "To location " << i << " : " << dist[i] << " units" << endl;
        }
    }
}

void inputMapManually(BellmanFordGraph &g)
{
    int V, E;
    cout << "Enter the number of locations: ";
    cin >> V;

    if (V <= 0)
    {
        cout << "Number of locations must be a positive integer." << endl;
        return;
    }

    cout << "Enter the number of roads: ";
    cin >> E;

    if (E < 0)
    {
        cout << "Number of roads must be a non-negative integer." << endl;
        return;
    }

    g = BellmanFordGraph(V, E);

    for (int i = 0; i < E; ++i)
    {
        int u, v, w;
        cout << "Road " << i + 1 << " (start end distance): ";
        cin >> u >> v >> w;

        if (u < 0 || u >= V || v < 0 || v >= V)
        {
            cout << "Invalid road. Location indices must be between 0 and " << V - 1 << "." << endl;
            return;
        }

        g.addEdge(u, v, w, i);
    }

    int src;
    cout << "Enter the starting location: ";
    cin >> src;

    if (src < 0 || src >= V)
    {
        cout << "Invalid starting location." << endl;
        return;
    }

    g.bellmanFord(src);
}

void loadMapFromFile(BellmanFordGraph &g)
{
    string filename = "bellManFord.txt";

    ifstream file(filename);
    if (!file)
    {
        cerr << "Unable to open file " << filename << endl;
        exit(1);
    }

    int V, E;
    file >> V >> E;

    if (V <= 0 || E < 0)
    {
        cout << "Invalid number of locations or roads in the file." << endl;
        return;
    }

    g = BellmanFordGraph(V, E);

    for (int i = 0; i < E; ++i)
    {
        int u, v, w;
        file >> u >> v >> w;

        if (u < 0 || u >= V || v < 0 || v >= V)
        {
            cout << "Invalid road in the file. Location indices must be between 0 and " << V - 1 << "." << endl;
            return;
        }

        g.addEdge(u, v, w, i);
    }

    file.close();

    int src;
    cout << "Enter the starting location: ";
    cin >> src;

    if (src < 0 || src >= V)
    {
        cout << "Invalid starting location." << endl;
        return;
    }

    g.bellmanFord(src);
}

void displayMainMenu()
{
    cout << "Main Menu:" << endl;
    cout << "1. Input map manually" << endl;
    cout << "2. Load map from file" << endl;
    cout << "3. Exit" << endl;
}

int main()
{

    while (true)
    {
        displayMainMenu();
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        BellmanFordGraph g(0, 0);

        switch (choice)
        {
        case 1:
            inputMapManually(g);
            break;
        case 2:
            loadMapFromFile(g);
            break;
        case 3:
            cout << "Exiting..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
            continue;
        }
    }

    return 0;
}
