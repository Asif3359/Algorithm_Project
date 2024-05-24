#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <fstream>
#include <sstream>
#include <chrono> // Include the <chrono> header

using namespace std;
using namespace std::chrono;

// Structure to represent a weighted edge in the graph
struct Edge
{
    int dest, weight;
};

// Class to represent a graph for Dijkstra's algorithm
class DijkstraGraph
{
public:
    int V;
    vector<vector<Edge>> adjList;

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

        for (const Edge &edge : adjList[u])
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

int main()
{
    while (true)
    {
        cout << "Main Menu:" << endl;
        cout << "1. Use Dijkstra's algorithm" << endl;
        cout << "2. Exit" << endl;

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 2)
        {
            cout << "Exiting..." << endl;
            return 0;
        }

        switch (choice)
        {
        case 1:
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
            break;
        }
        default:
            cout << "Invalid choice. Please try again." << endl;
            continue;
        }
    }

    return 0;
}
