#include <iostream>
#include <vector>
#include "GraphMethod.h"
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <list>
#include <utility>
#include <algorithm>
#define INF 1000000000 //infinity distance

using namespace std;

ofstream fout("log.txt", ios::app);

bool BFS(Graph* graph, char option, int vertex)
{
    {
    int size = graph->getSize();
    bool* visited = new bool [size]; //visited[size] is set to true when v is visited
    fill(visited, visited + size, false); //visited initialization

    visited[vertex] = true;
    queue<int> q;  //BFS uses queue
    q.push(vertex);
    map<int, int> adjEdge;
    fout << vertex;

    while (!q.empty()) {
        vertex = q.front(); //current point
        q.pop();
       
        if (option == 'Y') {
            graph->getAdjacentEdgesDirect(vertex, &adjEdge);
        }
        else {
            graph->getAdjacentEdges(vertex, &adjEdge);
        }

        for (const auto& pair : adjEdge) {
            if (!visited[pair.first] && fout << " -> ") {
                q.push(pair.first);
                visited[pair.first] = true;
                fout << pair.first;
            }
        }
    }
    delete[] visited;
    fout << endl;
    return true;
}
}

bool DFS(Graph* graph, char option, int vertex)
{
    int size = graph->getSize();
    bool* visited = new bool[size]; //visited[size] is set to true when v is visited
    fill(visited, visited + size, false); //visited initialization

    visited[vertex] = true;
    stack<int> s;  //DFS uses stack
    s.push(vertex);
    map<int, int> adjEdge;
    fout << vertex;

    while (!s.empty()) {
        vertex = s.top();
        s.pop();

        if (option == 'Y') {
            graph->getAdjacentEdgesDirect(vertex, &adjEdge);
        }
        else {
            graph->getAdjacentEdges(vertex, &adjEdge);
        }

        for (const auto& pair : adjEdge) {
            if (!visited[pair.first] && fout << " -> ") { //pair.first == nextVertex
                s.push(vertex);
                s.push(pair.first);
                visited[pair.first] = true;
                fout << pair.first;
                break;
            }
        }
    }
    delete[] visited;
    fout << endl;
    return true;
}

//=======================================================
struct edge {
    int from;
    int to;
    int weight;
};

void insertionSort(vector<edge>& arr, int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        edge key = arr[i];
        int j = i - 1;

        while (j >= low && arr[j].weight > key.weight) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int partition(vector<edge>& arr, int low, int high) {
    edge pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].weight <= pivot.weight) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<edge>& arr, int low, int high) {
    int segment_size = 7;
    if (low < high) {
        if (high - low + 1 <= segment_size) { //Sort by size
            insertionSort(arr, low, high);
        }
        else {
            int pivotIndex = partition(arr, low, high);
            quickSort(arr, low, pivotIndex - 1);
            quickSort(arr, pivotIndex + 1, high);
        }
    }
}
//=======================================================
//Used in Kruskal

int getParent(int a, int*parent) {
    if (parent[a] == a) {
        return a;
    }
    parent[a] = getParent(parent[a], parent);
}

bool findCycle(int from, int to, int*parent) {
    from = getParent(from, parent);
    to = getParent(to, parent);
    if (from == to) {
        return true;
    }
    else {
        return false;
    }
}

void unionSets(int from, int to, int*parent) {
    from = getParent(from, parent);
    to = getParent(to, parent);

    if (from < to) {
        parent[to] = from;
    }
    else {
        parent[from] = to;
    }
}

bool Kruskal(Graph* graph)
{
    if (!graph) {
        return false;
    }
    
    int cost = 0;
    int size = graph->getSize();
    vector<struct edge> edges;
    int* parent = new int [size+1];

    for (int i = 1; i <= size; i++) {
        parent[i] = i;
    }

    for (int i = 1; i <= size; i++) {
        map<int, int> adjEdges;
        graph->getAdjacentEdges(i, &adjEdges);

        for (const auto& edge : adjEdges) {
            edges.push_back({ i, edge.first, edge.second }); //edge storage
        }
    }

    quickSort(edges, 0, edges.size() - 1); //Sort edges by ascending weight
    
    fout << "======== KRUSKAL ========" << endl;
    vector<vector<pair<int, int>>> result(size + 1);
    for (int i = 0; i < edges.size(); i++) {
        if (!findCycle(edges[i].from, edges[i].to, parent)) {
            cost += edges[i].weight;
            unionSets(edges[i].from, edges[i].to, parent);
            result[edges[i].from].push_back({ edges[i].to, edges[i].weight });
            result[edges[i].to].push_back({ edges[i].from, edges[i].weight });
        }
    }
    
    
    for (int i = 1; i <= size; i++) { //Output in order of weight
        fout << "[" << i << "] ";
        for (const auto& edge : result[i]) {
            fout << edge.first << "(" << edge.second << ") ";
        }
        fout << endl;
    }
    fout << "cost: " <<  cost << endl;
    fout << "=========================" << endl;
    fout << endl;
    
    delete[] parent;
    return true;
}

bool Dijkstra(Graph* graph, char option, int vertex)
{
    int size = graph->getSize();
    int* dist = new int[size + 1]; //Save the shortest distance
    fill(dist, dist + size + 1, INF);
    dist[vertex] = 0; //Initialize starting point
    int* prev = new int[size + 1]; //For backtracking, go to the previous peak
    fill(prev, prev + size + 1, -1);

    //Save edges with the shortest distance using a priority queue (distance, vertex)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({ 0, vertex }); //Initialize starting point

    while (!pq.empty()) {
        int vCurr = pq.top().second; //Set current vertex as adjacent vertex
        int dCurr = pq.top().first; //Set current distance as adjacent vertex
        pq.pop();

        if (dCurr > dist[vCurr]) {
            continue;
        }

        map<int, int> adjEdges;
        if (option == 'Y') {
            graph->getAdjacentEdgesDirect(vCurr, &adjEdges);
        }
        else {
            graph->getAdjacentEdges(vCurr, &adjEdges);
        }

        for (const auto& edge : adjEdges) {
            int vNext = edge.first;
            int weight = edge.second;
            if (dist[vNext] > dist[vCurr] + weight) {
                dist[vNext] = dist[vCurr] + weight; //Update shortest distance
                prev[vNext] = vCurr; //Update prev path
                pq.push({ dist[vNext], vNext });
            }
        }
    }

    //Shortest path print
    for (int i = 1; i <= size; i++) {
        if (i != vertex) {
            fout << "[" << i << "] ";

            stack<int> path; //Shortest path backtracking
            int vCurr = i; //Current vertex
            while (vCurr != -1) {
                path.push(vCurr);
                vCurr = prev[vCurr];
            }

            fout << path.top();
            path.pop();
            while (!path.empty()) {
                fout << " -> " << path.top();
                path.pop();
            }

            fout << " (" << dist[i] << ")" << endl;
        }
    }
    fout << endl;
    delete[] dist;
    delete[] prev;
    return true;
}

bool Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex)
{
    int size = graph->getSize();
    int* dist = new int[size+1]; //Save minimum distance
    fill(dist, dist + size + 1, INF);
    dist[s_vertex] = 0;
    int* prev = new int[size+1]; //prev path
    fill(prev, prev + size + 1, s_vertex);
    
    vector<struct edge> edges;
    
    for (int i = 1; i <= size; i++) {
        map<int, int> adjEdges;
        if (option == 'Y') {
            graph->getAdjacentEdgesDirect(i, &adjEdges);
        }
        else {
            graph->getAdjacentEdges(i, &adjEdges);
        }

        for (const auto& edge : adjEdges) {
            edges.push_back({ i, edge.first, edge.second }); //edge storage
        }
    }
    
    
    for (int i = 2; i < size; i++) {
        for (int j = 0; j < edges.size(); j++) {
            int from = edges[j].from;
            int to = edges[j].to;
            int cost = edges[j].weight;

            if (dist[from] == INF) {
                continue;
            }

            if (dist[to] > dist[from] + cost) { //Select minimum distance
                dist[to] = dist[from] + cost;
                prev[to] = from;
               
                if (i == size) { //negative cycle
                    return false;
                }
            }
        }
    }

    if (dist[e_vertex] == INF) { //In case of infinite distance
        fout << "x" << endl;
        return true;
    }
    else {
        fout << s_vertex;
        vector<int>path;
        for (int i = e_vertex; i != s_vertex; i = prev[i]) {
            path.push_back(i);
        }
        for (int i = path.size() - 1; i >= 0; --i) {
            fout << " -> " << path[i];
        }
    }
    fout << endl;
    fout << "cost : " << dist[e_vertex] << endl;
    
    delete[] dist;
    delete[] prev;
    return true;
}

bool FLOYD(Graph* graph, char option)
{
    int size = graph->getSize();
    int** dist = new int* [size + 1];
    for (int i = 1; i <= size; i++) {
        dist[i] = new int[size + 1];
    }
    
    for (int i = 1; i <= size; i++) {
        map<int, int> adjEdges;
        if (option == 'Y') {
            graph->getAdjacentEdgesDirect(i, &adjEdges);
        }
        else {
            graph->getAdjacentEdges(i, &adjEdges);
        }

        for (int j = 1; j <= size; j++) {
            if (i == j) {
                dist[i][j] = 0; //Initializing self matrix
            }
            else if (adjEdges.find(j) != adjEdges.end()) {
                dist[i][j] = adjEdges[j]; //If an edge exists
            }
            else {
                dist[i][j] = INF; //When edge does not exist
            }
        }
    }

    for (int k = 1; k <= size; k++) {
        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
            if (dist[i][i] < 0) { //negative cycle
                return false;
            }
        }
    }

   //print
    fout << "    ";
    for (int i = 1; i <= size; i++) {
        fout << "[" << i << "] ";
    }
    fout << endl;
    for (int i = 1; i <= size; i++) {
        fout << "[" << i << "]  ";
        for (int j = 1; j <= size; j++) {
            if (dist[i][j] == INF) { //In case of infinite distance
                fout << "x   ";
            }
            else {
                fout << dist[i][j] <<"   ";
            }
        }
        fout << endl;
    }

    return true;
}

bool KWANGWOON(Graph* graph) {

    return false;
}