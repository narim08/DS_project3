#include "ListGraph.h"
#include <iostream>
#include <utility>

ListGraph::ListGraph(bool type, int size) : Graph(type, size)
{
	m_List = new map<int, int>[size+1];
}

ListGraph::~ListGraph()
{
	delete[] m_List;
}

void ListGraph::getAdjacentEdges(int vertex, map<int, int>* m)	 //Definition of getAdjacentEdges(No Direction == Undirected)
{
	if (vertex >= 1 && vertex <= m_Size) {
		m->clear();

		//Priority queue for storing each weight
		priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

		for (const auto& edge : m_List[vertex]) {
			pq.push(make_pair(edge.second, edge.first));
		}

		//Insert into map in order of weight
		while (!pq.empty()) {
			pair<int, int> edge = pq.top();
			pq.pop();
			(*m)[edge.second] = edge.first;
		}
	}
}

void ListGraph::getAdjacentEdgesDirect(int vertex, map<int, int>* m) //Definition of getAdjacentEdges(Directed graph)
{
	(*m) = m_List[vertex];
}

void ListGraph::insertEdge(int from, int to, int weight) //Definition of insertEdge
{
	if (from <= m_Size && to <= m_Size) {
		m_List[from].insert({to, weight});
	}
}

bool ListGraph::printGraph(ofstream* fout)	//Definition of print Graph
{
	if (!fout->is_open()) {
		return false;
	}

	*fout << "========PRINT========" << endl;
	for (int i = 1; i <= m_Size; i++) {
		*fout <<"[" << i << "] ";

		for (auto pair = m_List[i].begin(); pair != m_List[i].end() && *fout <<"->"; pair++) {
			*fout << " (" << pair->first << "," << pair->second << ") ";
			
		}
		*fout << endl;
	}
	*fout << "=====================" << endl;
	*fout << endl;
	return true;
}