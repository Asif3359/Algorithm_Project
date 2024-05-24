#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>
#include <cctype>
#include <chrono>

using namespace std;

string toLower(const string &str)
{
    string lowerStr;
    transform(str.begin(), str.end(), back_inserter(lowerStr), ::tolower);
    return lowerStr;
}

// dijkstra

class DijkstraGraph
{
public:
    void addEdge(const string &u, const string &v, int weight)
    {
        string uLower = toLower(u);
        string vLower = toLower(v);
        adjList[uLower].push_back({vLower, weight});
        adjList[vLower].push_back({uLower, weight});
    }

    unordered_map<string, int> dijkstra(const string &src)
    {
        string srcLower = toLower(src);

        unordered_map<string, int> dist;
        for (auto &pair : adjList)
        {
            dist[pair.first] = INT_MAX;
        }
        dist[srcLower] = 0;

        using pii = pair<int, string>;
        priority_queue<pii, vector<pii>, greater<pii>> pq;
        pq.push({0, srcLower});

        while (!pq.empty())
        {
            int currentDist = pq.top().first;
            string u = pq.top().second;
            pq.pop();

            if (currentDist > dist[u])
                continue;

            for (auto &neighbor : adjList[u])
            {
                string v = neighbor.first;
                int weight = neighbor.second;

                if (dist[u] + weight < dist[v])
                {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }

        return dist;
    }

private:
    unordered_map<string, vector<pair<string, int>>> adjList;
};

// bellman-ford

struct bellEdge
{
    string source, destination;
    int weight;
};

class Bellmanford
{
    int V;
    vector<bellEdge> edges;
    unordered_map<string, int> cityIndices;

public:
    Bellmanford(int vertices) : V(vertices) {}

    void addEdge(string u, string v, int w)
    {
        edges.push_back({u, v, w});
    }

    void printSolution(const vector<int> &dist, string src)
    {
        // Create a vector of pairs (city, distance)
        vector<pair<string, int>> distances;
        for (const auto &cityIndexPair : cityIndices)
        {
            string dest = cityIndexPair.first;
            int index = cityIndexPair.second;
            distances.push_back({dest, dist[index]});
        }

        // Sort distances in increasing order
        sort(distances.begin(), distances.end(), [](const pair<string, int> &a, const pair<string, int> &b)
             { return a.second < b.second; });

        cout << "Shortest distances from " << src << " to all other cities in increasing order:\n";
        for (const auto &distance : distances)
        {
            cout << "Distance to " << distance.first << ": " << distance.second << endl;
        }
    }

    void bellmanFord(string src)
    {
        vector<int> dist(V, INT_MAX);
        dist[cityIndices[toLower(src)]] = 0;
        // Convert src to lowercase
        auto start = chrono::high_resolution_clock::now(); // Start the timer

        for (int i = 0; i < V - 1; ++i)
        {
            for (const auto &edge : edges)
            {
                string u = edge.source;
                string v = edge.destination;
                int weight = edge.weight;
                if (dist[cityIndices[toLower(u)]] != INT_MAX && dist[cityIndices[toLower(u)]] + weight < dist[cityIndices[toLower(v)]])
                {
                    dist[cityIndices[toLower(v)]] = dist[cityIndices[toLower(u)]] + weight;
                }
            }
        }
        auto end = chrono::high_resolution_clock::now();
        printSolution(dist, src);

        auto duration = chrono::duration_cast<chrono::microseconds>(end - start); // Calculate the duration in microseconds
        cout << "Time taken: " << duration.count() << " microseconds" << endl;    // Output the time taken
    }

    void setCityIndices(const vector<string> &cities)
    {
        for (int i = 0; i < V; ++i)
        {
            cityIndices[toLower(cities[i])] = i;
        }
    }
};

// dijkstra
void loadFileFromDijkstra()
{
    DijkstraGraph g;
    ifstream inputFile("socialList.txt");
    if (!inputFile.is_open())
    {
        cerr << "Unable to open file.\n";
        return;
    }

    int numUsers, numConnections;
    inputFile >> numUsers >> numConnections;
    inputFile.ignore(); // Ignore remaining newline character

    for (int i = 0; i < numConnections; ++i)
    {
        string source, destination;
        int distance;
        inputFile >> source >> destination >> distance;
        g.addEdge(source, destination, distance);
    }

    string srcUser;
    inputFile.ignore(); // Ignore remaining newline character
    getline(inputFile, srcUser);

    inputFile.close();

    unordered_map<string, int> distances = g.dijkstra(srcUser);

    cout << "Shortest paths from " << srcUser << " to all destinations (sorted by distance):\n";
    vector<pair<string, int>> sortedDistances;
    for (const auto &pair : distances)
    {
        sortedDistances.push_back(pair);
    }
    sort(sortedDistances.begin(), sortedDistances.end(), [](const auto &a, const auto &b)
         { return a.second < b.second; });

    for (const auto &pair : sortedDistances)
    {
        cout << "To " << pair.first << ": " << pair.second << '\n';
    }
}

void loadManualyDijkstra()
{

    DijkstraGraph g;
    int numUsers, numConnections;

    cout << "Enter number of users: ";
    cin >> numUsers;

    cout << "Enter number of connections: ";
    cin >> numConnections;

    cin.ignore(); // Ignore remaining newline character

    cout << "Enter connections (source destination distance):\n";
    for (int i = 0; i < numConnections; ++i)
    {
        string source, destination;
        int distance;
        getline(cin, source, ' ');
        getline(cin, destination, ' ');
        cin >> distance;
        cin.ignore(); // Ignore remaining newline character
        g.addEdge(source, destination, distance);
    }

    string srcUser;
    cout << "Enter source username: ";
    getline(cin, srcUser);

    unordered_map<string, int> distances = g.dijkstra(srcUser);

    cout << "Shortest paths from " << srcUser << " to all destinations (sorted by distance):\n";
    vector<pair<string, int>> sortedDistances;
    for (const auto &pair : distances)
    {
        sortedDistances.push_back(pair);
    }
    sort(sortedDistances.begin(), sortedDistances.end(), [](const auto &a, const auto &b)
         { return a.second < b.second; });

    for (const auto &pair : sortedDistances)
    {
        cout << "To " << pair.first << ": " << pair.second << '\n';
    }
}
// bellmen-ford
void inputManuallyBellmenFord()
{
    int numCities, numRoads;
    cout << "Enter the number of cities: ";
    cin >> numCities;

    vector<string> cities(numCities);
    cout << "Enter the names of cities:\n";
    for (int i = 0; i < numCities; ++i)
    {
        cin >> cities[i];
    }

    cout << "Enter the number of roads: ";
    cin >> numRoads;

    Bellmanford g(numCities);
    g.setCityIndices(cities);

    cout << "Enter start city, end city, and length for each road:\n";
    for (int i = 0; i < numRoads; ++i)
    {
        string startCity, endCity;
        int length;
        cin >> startCity >> endCity >> length;
        g.addEdge(startCity, endCity, length);
    }

    string startCity;
    cout << "Enter the start city: ";
    cin >> startCity;

    g.bellmanFord(toLower(startCity));
}

void loadfromFileBellmenFord()
{
    ifstream inputFile("citysList.txt");
    if (!inputFile)
    {
        cerr << "Error: Unable to open input file." << endl;
        return;
    }

    int numCities, numRoads;
    inputFile >> numCities;

    vector<string> cities(numCities);
    for (int i = 0; i < numCities; ++i)
    {
        inputFile >> cities[i];
    }

    inputFile >> numRoads;

    Bellmanford g(numCities);
    g.setCityIndices(cities);

    for (int i = 0; i < numRoads; ++i)
    {
        string startCity, endCity;
        int length;
        inputFile >> startCity >> endCity >> length;
        g.addEdge(startCity, endCity, length);
    }

    string startCity;
    inputFile >> startCity;

    g.bellmanFord(toLower(startCity));

    inputFile.close();
}

// gps application
void gpsApplication()
{
    while (true)
    {
        char choice;
        cout << "\n\t\tGPS Navigation\t\t\n";
        cout << "Do you want to\n";
        cout << "1. Input data manually \n";
        cout << "2. Input from a file \n";
        cout << "3. Go to Main\n";
        cout << "Enter your choice : ";

        cin >> choice;
        system("cls");
        switch (choice)
        {
        case '1':
        {
            inputManuallyBellmenFord();
            break;
        }
        case '2':
        {
            loadfromFileBellmenFord();
            break;
        }
        case '3':
        {
            return;
        }
        default:
            cout << "Invalid choice." << endl;
            continue;
        }
    }
}

void socialNetApplication()
{

    while (true)
    {
        char inputChoice;
        cout << "\n\t\tSocial Network\t\t\n";
        cout << "Do you want to \n";
        cout << "1. Input Manualy \n";
        cout << "2. input from file \n";
        cout << "3. Go to main\n";
        cout << "Enter your choice : ";
        cin >> inputChoice;
        system("cls");
        switch (inputChoice)
        {
        case '1':
            loadManualyDijkstra();
            break;
        case '2':
            loadFileFromDijkstra();
            break;
        case '3':
            return;
        default:
            cout << "Invalid choice. " << endl;
            continue;
        }
    }
}

int main()
{
    while (true)
    {
        system("cls");
        cout << "\n\t\tMain\t\t\n";
        cout << "Do you want use \n";
        cout << "1. GPS navigations(Bellman-ford)\n";
        cout << "2. Social network (Dijkstra's) \n";
        cout << "3. Exit program\n";
        cout << "Enter your choice : ";
        char ch;
        cin >> ch;
        system("cls");
        switch (ch)
        {
        case '1':
            gpsApplication();
            break;
        case '2':
            socialNetApplication();
            break;
        case '3':
            return 0;
        default:
            cout << "Envilid choice ...";
            continue;
        }
    }

    return 0;
}
