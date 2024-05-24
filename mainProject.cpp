#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <fstream>
#include <chrono>
#include <sstream>

using namespace std;
using namespace std::chrono;

// dijkstra's
struct DijEdge
{
    int dest, weight;
};

class DijkstraGraph
{
public:
    int V;
    vector<vector<DijEdge>> adjList;

    DijkstraGraph(int V)
    {
        this->V = V;
        adjList.resize(V);
    }

    void addEdge(int u, int v, int weight)
    {
        adjList[u].push_back({v, weight});
    }

    void dijkstra(int src);
};

void DijkstraGraph::dijkstra(int src)
{
    vector<int> dist(V, INT_MAX);
    dist[src] = 0;

    auto start = high_resolution_clock::now(); // Start measuring time

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, src});

    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();

        for (const DijEdge &edge : adjList[u])
        {
            int v = edge.dest;
            int weight = edge.weight;

            if (dist[u] + weight < dist[v])
            {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start).count();

    cout << "Time taken by Dijkstra's algorithm: " << duration << " microseconds" << endl;

    cout << "Shortest distances from user " << src << " to all other users:" << endl;
    for (int i = 0; i < V; ++i)
    {
        if (dist[i] == INT_MAX)
        {
            cout << "User " << i << " is unreachable" << endl;
        }
        else
        {
            cout << "To user " << i << " : " << dist[i] << endl;
        }
    }
}

// bellman-ford
struct bellEdges
{
    int src, dest, weight;
};

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

// bellman-ford
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

void displayBellmenFord()
{
    cout << "Main Menu:" << endl;
    cout << "1. Input map manually" << endl;
    cout << "2. Load map from file" << endl;
    cout << "3. Exit" << endl;
}

// dijkstra
void inputGraphManually()
{
    int V, E;
    cout << "Enter the number of users (vertices): ";
    cin >> V;

    cout << "Enter the number of connections (edges): ";
    cin >> E;

    DijkstraGraph g(V);

    for (int i = 0; i < E; ++i)
    {
        int u, v, w;
        cout << "Connection " << i + 1 << " (user1 user2 weight): ";
        cin >> u >> v >> w;
        g.addEdge(u, v, w);
    }

    int src;
    cout << "Enter the source user: ";
    cin >> src;

    g.dijkstra(src);
}

void loadGraphFromFile()
{
    const string filename = "dijkstraInput.txt";

    ifstream file(filename);
    if (!file)
    {
        cerr << "Unable to open file " << filename << endl;
        return; // Return gracefully if file cannot be opened
    }

    int V, E;
    file >> V >> E;
    DijkstraGraph g(V);

    for (int i = 0; i < E; ++i)
    {
        int u, v, w;
        file >> u >> v >> w;
        g.addEdge(u, v, w);
    }

    file.close();

    int src;
    cout << "Enter the source user: ";
    cin >> src;

    g.dijkstra(src);
}

void displayDijkstraMenu()
{
    cout << "Dijkstra's Menu:" << endl;
    cout << "1. Input social network manually" << endl;
    cout << "2. Input social network from file" << endl;
    cout << "3. Back to main menu" << endl;
}

//bellman ford main
void bellmanfordMain()
{
     while (true)
            {
                displayBellmenFord();
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
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                    continue;
                }
            }
}

void dijkstraMain()
{
    while (true)
            {
                displayDijkstraMenu();

                int ch;
                cout << "Enter your choice: ";
                cin >> ch;

                switch (ch)
                {
                case 1:
                    inputGraphManually();
                    break;
                case 2:
                    loadGraphFromFile();
                    break;
                case 3:
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                    continue;
                }
                break;
            }
}

int main()
{
    while (true)
    {
        cout << "Main Menu:" << endl;
        cout << "1. Use GPS" << endl;
        cout << "2. Use Network" << endl;
        cout << "3. Exit" << endl;

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 3)
        {
            cout << "Exiting..." << endl;
            return 0;
        }

        switch (choice)
        {
        case 1:
        {

           bellmanfordMain();
           break;
        }
        case 2:
        {
            dijkstraMain();
            break;
        }
        default:
            cout << "Invalid choice. Please try again." << endl;
            continue;
        }
    }

    return 0;
}
