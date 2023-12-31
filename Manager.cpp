#include "Manager.h"
#include "GraphMethod.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

Manager::Manager()
{
	graph = nullptr;
	fout.open("log.txt", ios::app);
	load = 0;	//Anything is not loaded
}

Manager::~Manager()
{
	if (load)	//if graph is loaded, delete graph
		delete graph;
	if (fout.is_open())	//if fout is opened, close file
		fout.close();	//close log.txt File
}

void Manager::run(const char* command_txt) {
	ifstream fin;	//Command File Input File Stream
	fin.open(command_txt, ios_base::in);//File open with read mode

	if (!fin) { //If command File cannot be read, Print error
		fout << "command file open error" << endl;
		return;	//Return
	}

	char fstr[100];
	while (fin.getline(fstr, sizeof(fstr))) { // Read file line by line
		string line(fstr);
		istringstream iss(line);

		string cmd;
		char option;
		int vertex, s_vertex, e_vertex;
		char* file;

		iss >> cmd;

		if (cmd == "LOAD") {
			char* ptr = strtok(fstr, " ");
			cmd = ptr;
			ptr = strtok(NULL, " ");
			file = ptr;
			if (LOAD(file)) {
				printSuccessCode("LOAD");
			}
			else {
				printErrorCode(100);
			}
		}
		else if (cmd == "PRINT") {
			if (!PRINT()) {
				printErrorCode(200);
			}
		}
		else if (cmd == "BFS") {
			char* ptr = strtok(fstr, " ");
			cmd = ptr;
			ptr = strtok(NULL, " ");
			option = ptr[0];
			ptr = strtok(NULL, " ");
			vertex = atoi(ptr);
			if (!mBFS(option, vertex)) {
				printErrorCode(300);
			}
		}
		else if (cmd == "DFS") {
			char* ptr = strtok(fstr, " ");
			cmd = ptr;
			ptr = strtok(NULL, " ");
			option = ptr[0];
			ptr = strtok(NULL, " ");
			vertex = atoi(ptr);
			if (!mDFS(option, vertex)) {
				printErrorCode(400);
			}
		}
		else if (cmd == "KRUSKAL") {
			if (!mKRUSKAL()) {
				printErrorCode(600);
			}
		}
		else if (cmd == "DIJKSTRA") {
			char* ptr = strtok(fstr, " ");
			cmd = ptr;
			ptr = strtok(NULL, " ");
			option = ptr[0];
			ptr = strtok(NULL, " ");
			vertex = atoi(ptr);
			if (!mDIJKSTRA(option, vertex)) {
				printErrorCode(700);
			}
		}
		else if (cmd == "BELLMANFORD") {
			char* ptr = strtok(fstr, " ");
			cmd = ptr;
			ptr = strtok(NULL, " ");
			option = ptr[0];
			ptr = strtok(NULL, " ");
			s_vertex = atoi(ptr);
			ptr = strtok(NULL, " ");
			e_vertex = atoi(ptr);
			if (!mBELLMANFORD(option, s_vertex, e_vertex)) {
				printErrorCode(800);
			}
		}
		else if (cmd == "FLOYD") {
			char* ptr = strtok(fstr, " ");
			cmd = ptr;
			ptr = strtok(NULL, " ");
			option = ptr[0];
			if (!mFLOYD(option)) {
				printErrorCode(900);
			}
		}
		else if (cmd == "KWANGWOON") {
			char* ptr = strtok(fstr, " ");
			cmd = ptr;
			if (!mKwoonWoon()) {
				printErrorCode(500);
			}
		}
		else if (cmd == "EXIT") {
			printSuccessCode("EXIT");
		}
		else { //Incorrect command
			printErrorCode(1000);
		}
	}
	fin.close();
	return;
}

bool Manager::LOAD(const char* filename)
{
	ifstream fdata(filename); //open data file
	if (!fdata) { //Error if file does not exist
		return false;
	}
	if (load) { //Delete existing graph
		delete graph;
	}

	int mType = 0;
	int mSize = 0;
	int to = 0;
	int from = 0;
	int weight = 0;
	string str;

	string type;
	char fstr[100];
	while (fdata.getline(fstr, sizeof(fstr))) { //type checking
		string line(fstr);
		istringstream iss(line);
		iss >> type;
		break;
	}

	string size;
	if (type == "L" || type=="M") { //size checking
		while (fdata.getline(fstr, sizeof(fstr))) {
			string line(fstr);
			istringstream iss(line);
			iss >> size;
			mSize = stoi(size);
			break;
		}
	}
	else { //invalid type
		return false;
	}

	if (type == "L") { //List graph
		graph=new ListGraph(0, mSize); //Create a new List graph
		while (fdata.getline(fstr, sizeof(fstr))) {
			string ls;
			ls = fstr;
			if (ls.size()==2 || ls.size()==3) { //If there is 1 argument, store it in from
				from = stoi(ls);
				continue;
			}
			else { //to, weight
				string line(fstr); //Separate storage based on space
				istringstream iss(line);
				iss >> to >> weight;
				graph->insertEdge(from,to,weight);
			}
		}
	}
	else if(type=="M"){ //Matrix graph
		graph = new MatrixGraph(1, mSize); //Create a new Matrix graph
		for(from=1; from<=mSize; from++){ //row
			for(to=1; to<=mSize; to++){ //column
				if(to == mSize) { //Skip to next line
					getline(fdata, str);
				}
				else{
					getline(fdata, str, ' ');
				}

				weight = stoi(str);
				if(weight){ //Insert if connection exists
					graph->insertEdge(from, to, weight);
				}
			}
		}
	}

	load++;
	return true;
}

bool Manager::PRINT()
{
	if (!graph) { //When the graph does not exist
		return false;
	}
	graph->printGraph(&fout);
	
	return true;
}

bool Manager::mBFS(char option, int vertex)
{
	if (!graph || vertex > graph->getSize()) { //graph does not exist or no starting point
		return false;
	}
	if (option != 'Y' && option != 'N') { //invalid argument
		return false;
	}

	fout << "========BFS========" << endl;
	if (option == 'Y') {
		fout << "Directed Graph BFS result" << endl;
	}
	else if (option == 'N') {
		fout << "Undirected Graph BFS result" << endl;
	}
	fout << "startvertex: " << vertex << endl;
	BFS(graph, option, vertex);
	fout << "===================" << endl;
	fout << endl;
	return true;
}

bool Manager::mDFS(char option, int vertex)
{
	if (!graph || vertex > graph->getSize()) { //graph does not exist or no starting point
		return false;
	}
	if (option != 'Y' && option != 'N') { //invalid argument
		return false;
	}

	fout << "========DFS========" << endl;
	if (option == 'Y') {
		fout << "Directed Graph DFS result" << endl;
	}
	else if (option == 'N') {
		fout << "Undirected Graph DFS result" << endl;
	}
	fout << "startvertex: " << vertex << endl;
	DFS(graph, option, vertex);
	fout << "===================" << endl;
	fout << endl;
	return true;
}

bool Manager::mDIJKSTRA(char option, int vertex)
{
	if (!graph || vertex > graph->getSize()) { //graph does not exist
		return false;
	}
	fout << "======== DIJKSTRA ========" << endl;
	if (option == 'Y') {
		fout << "Directed Graph Dijkstra result" << endl;
	}
	else if (option == 'N') {
		fout << "Undirected Graph Dijkstra result" << endl;
	}
	fout << "startvertex: " << vertex << endl;
	Dijkstra(graph, option, vertex);
	fout << "==========================" << endl;
	fout << endl;
	return true;
}

bool Manager::mKRUSKAL()
{
	if (!graph || !Kruskal(graph)) { //graph does not exist
		return false;
	}
	return true;
}

bool Manager::mBELLMANFORD(char option, int s_vertex, int e_vertex)
{
	if (!graph) {
		return false;
	}
	fout << "======== Bellman-Ford ========" << endl;
	if (option == 'Y') {
		fout << "Directed Graph Bellman-Ford result" << endl;
	}
	else if (option == 'N') {
		fout << "Undirected Graph Bellman-Ford result" << endl;
	}
	Bellmanford(graph, option, s_vertex, e_vertex);
	fout << "==========================" << endl;
	fout << endl;
	return true;
}

bool Manager::mFLOYD(char option)
{
	if (!graph) {
		return false;
	}
	fout << "======== FLOYD ========" << endl;
	if (option == 'Y') {
		fout << "Directed Graph FLOYD result" << endl;
	}
	else if (option == 'N') {
		fout << "Undirected Graph FLOYD result" << endl;
	}
	FLOYD(graph, option);
	fout << "==========================" << endl;
	fout << endl;
	return true;
}

bool Manager::mKwoonWoon() {
	if (!graph || !KWANGWOON(graph)) {
		return false;
	}
	
	return true;
}

void Manager::printErrorCode(int n) //ERROR CODE PRINT
{
	fout << "========ERROR=======" << endl;
	fout << n << endl;
	fout << "====================" << endl << endl;
}

void Manager::printSuccessCode(const char* cmd) { //SUCCESS CODE PRINT 
	fout << "========" << cmd << "========" << endl;
	fout << "Success" << endl;
	fout << "====================" << endl << endl;
}
