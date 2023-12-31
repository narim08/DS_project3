#include "MatrixGraph.h"
#include <iostream>
#include <vector>
#include <string>

struct edge {
    int from;
    int to;
    int weight;
};

MatrixGraph::MatrixGraph(bool type, int size) : Graph(type, size)
{
    m_Mat = new int* [size+1];
    for (int i = 1; i <= size; i++) {
        m_Mat[i] = new int[size+1];
        memset(m_Mat[i], 0, sizeof(int) * (size+1)); //Matrix initialization
    }
}

MatrixGraph::~MatrixGraph()
{
    for (int i = 1; i <= m_Size; i++) {
        delete[] m_Mat[i];
    }
    delete[] m_Mat;
}

void MatrixGraph::getAdjacentEdges(int vertex, map<int, int>* m)
{
    if (vertex >= 1 && vertex <= m_Size) {
        m->clear();

        vector<struct edge> edges; //Two-way storage
        for (int i = 1; i <= m_Size; i++) {
            if (m_Mat[vertex][i] != 0) {
                edges.push_back({ vertex, i, m_Mat[vertex][i] });
            }
            if (m_Mat[i][vertex] != 0) {
                edges.push_back({ i, vertex, m_Mat[i][vertex] });
            }
        }

        for (const auto& edge : edges) { //insert into map
            (*m)[edge.to] = edge.weight;
        }
    }
}

void MatrixGraph::getAdjacentEdgesDirect(int vertex, map<int, int>* m)
{
    if (vertex >= 1 && vertex <= m_Size) {
        m->clear();

        vector<pair<int, int>> edges; //Save outgoing edge
        for (int i = 1; i <= m_Size; i++) {
            if (m_Mat[vertex][i] != 0) {
                edges.push_back({ i, m_Mat[vertex][i] });
            }
        }

        for (const auto& edge : edges) { //insert into map
            (*m)[edge.first] = edge.second;
        }
    }
}


void MatrixGraph::insertEdge(int from, int to, int weight)
{
    if (from <= m_Size && to <= m_Size) {
        m_Mat[from][to] = weight;
    }
}

bool MatrixGraph::printGraph(ofstream* fout)
{
    if (!fout->is_open()) {
        return false;
    }

    *fout << "========PRINT========" << endl;
    *fout << "    ";
    for (int i = 1; i <= m_Size; i++) {
        *fout << "[" << i << "]" << " ";
    }
    *fout << endl;

    for (int i = 1; i <= m_Size; i++) {
        *fout << "[" << i << "]" << "  ";

        for (int j = 1; j <= m_Size; j++) {
            *fout << m_Mat[i][j] << "   ";
        }
        *fout << endl;
    }
   
    *fout << "=====================" << endl;
    *fout << endl;

    return true;
}