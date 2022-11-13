#pragma once
#include "Graph.h"


inline int Graph::minDistance(int dist[], bool sptSet[]) const
{
	int min = INT_MAX, min_index = 0;

	for (int v = 0; v < this->nodes; v++)
		if (sptSet[v] == false && dist[v] <= min)
			min = dist[v], min_index = v;

	return min_index;
}

inline std::vector<std::string> Graph::split_by_delimiter(std::string text)
{
	std::vector<std::string> words{};
	size_t pos = 0;
	while ((pos = text.find('#')) != std::string::npos) {
		words.push_back(text.substr(0, pos));
		text.erase(0, pos + 1);
	}
	words.push_back(text);
	return words;
}

inline int Graph::find(int i)
{
	if (parent[i] == i)
		return i;
	return parent[i] = find(parent[i]);
}

inline void Graph::union1(int i, int j)
{
	int a = find(i);
	int b = find(j);
	if (a != b) {
		if (rank[a] < rank[b]) {
			parent[a] = b;
			rank[b] += rank[a];
		}
		else {
			parent[b] = a;
			rank[a] += rank[b];
		}
	}
	//this->parent[a] = b;
}

inline int Graph::findIndex(std::string find) const
{
	for (int i = 0; i < this->NodeList.size(); i++)
		if (this->NodeList[i]->value == find)
			return i;
	return -1;
}

inline void Graph::makeMatrix()
{
	//make matrix
	this->nodes += 1;

	std::vector<int> temp;
	for (int i = 0; i < this->nodes; i++)
		this->matrix.push_back(temp);
	for (int i = 0; i < this->nodes; i++)
		for (int x = 0; x < this->nodes; x++)
			this->matrix[i].push_back(INT_MAX);

	//remake the graph to a matrix
	for (int i = 0; i < this->nodes - 1; i++)
	{
		for (std::pair<int, Node*> y : this->NodeList[i]->next)
		{
			int index = findIndex(y.second->value);
			this->matrix[i][index] = y.first;
		}
	}
	this->nodes--;
}

inline Graph::Graph(std::string filename)
{

	std::ifstream File;
	if (std::ifstream File{ "./Textfiler_med_grafer/" + filename })
	{
		std::string line;
		std::vector<std::string> conections;
		while (getline(File, line))
		{
			if (line.size() != 0)
			{
				if (line.find('#') != std::string::npos)
				{
					conections.push_back(line);
				}
				else
				{
					this->NodeList.push_back(new Node(line));
				}
			}
		}
		this->nodes = this->NodeList.size();
		this->parent = new int[this->NodeList.size()];
		this->rank = new int[this->NodeList.size()];
		for (std::string node : conections)
		{
			std::vector<std::string> conections_list = split_by_delimiter(node);
			for (Node* x : this->NodeList)
			{
				if (x->value == conections_list[0])
				{
					Node* next_node = nullptr;
					for (Node* x : this->NodeList)
					{
						if (x->value == conections_list[1])
						{
							next_node = x;
							break;
						}
					}
					std::pair<int, Node*> temp;
					temp.first = std::stoi(conections_list[2]);
					temp.second = next_node;
					x->next.push_back(temp);
					this->edges++;
					break;
				}
			}
		}
	}
	else
	{
		std::cout << "fili cant open!" << std::endl;
	}
	makeMatrix();
	File.close();
}

inline Graph::~Graph()
{
	for (Node* x : this->NodeList)
	{
		delete x;
	}
	delete[] this->parent;
	this->parent = nullptr;
	delete[] this->rank;
	this->rank = nullptr;
	//radera wallah
}

inline int Graph::nrOfNodes() const
{
	return this->nodes;
}

inline int Graph::nrOfEdges() const
{
	return this->edges;
}

inline void Graph::printPath(int parent[], int j, std::vector<int>& returnVect) const
{
	if (parent[j] < 0)
		return;
	printPath(parent, parent[j], returnVect);
	returnVect.push_back(j);

}

inline int Graph::shortestPath(std::string& from, std::string& to, std::vector<std::pair<int, std::string>>& shortestPath) const//Dijktras BFS
{
	int* parent = new int[this->nodes];
	int* dist = new int[this->nodes];
	bool* sptSet = new bool[this->nodes];

	for (int i = 0; i < this->nodes; i++)
		dist[i] = INT_MAX, sptSet[i] = false;

	dist[findIndex(from)] = 0;

	for (int count = 0; count < this->nodes; count++) {

		int currentNode = minDistance(dist, sptSet);
		sptSet[currentNode] = true;

		for (int nextNode = 0; nextNode < this->nodes; nextNode++)
		{
			if (!sptSet[nextNode] && this->matrix[currentNode][nextNode] && dist[currentNode] + this->matrix[currentNode][nextNode] < dist[nextNode] && this->matrix[currentNode][nextNode] != INT_MAX)
			{
				parent[nextNode] = currentNode;
				dist[nextNode] = dist[currentNode] + this->matrix[currentNode][nextNode];
			}
		}
	}
	//hitta vägen
	std::vector<int> thePath;
	std::pair<int, std::string> temp;
	printPath(parent, findIndex(to), thePath);
	if (thePath.size() > 0)
	{
		temp.first = 0;
		temp.second = this->NodeList[findIndex(from)]->value;
		shortestPath.push_back(temp);
	}

	for (int i = 0; i < thePath.size(); i++)
	{
		if (dist[thePath[i]] > 0)
		{
			temp.first = dist[thePath[i]];
			temp.second = this->NodeList[thePath[i]]->value;
			shortestPath.push_back(temp);
		}
		else
		{
			shortestPath.clear();
			break;
		}
	}
	delete[] parent;
	delete[] dist;
	delete[] sptSet;
	if (shortestPath.size() > 0)
		return shortestPath[shortestPath.size() - 1].first;
	return 0;
}

inline std::vector<std::pair<int, std::pair<std::string, std::string>>> Graph::adj_maker()
{
	std::pair<int, std::pair<std::string, std::string>> v;
	std::vector<std::pair<int, std::pair<std::string, std::string>>> returnobj;
	for (auto t : this->NodeList) {
		for (auto n : t->next) {
			v.second.first = t->value;
			v.second.second = n.second->value;
			v.first = n.first;
			returnobj.push_back(v);
		}
	}
	std::sort(returnobj.begin(), returnobj.end());
	return returnobj;
}
inline int Graph::kruskalMST(std::vector<std::pair<int, std::string>>& returnObj)
{

	// gammla kod


	int mincost = 0; // lägsta kostnaden av MST.
	int V = this->nodes;
	////initierar ett värde så det är inte är ett int min värde 
	for (int i = 0; i < V; i++) {
		this->parent[i] = i;
		this->rank[i] = 0;
	}

	//nya
	std::vector<std::pair<int, std::pair<std::string, std::string>>> adj = adj_maker();
	int ans = 0;
	for (auto edge : adj) {
		int w = edge.first;
		std::string x = edge.second.first;
		std::string y = edge.second.second;

		// Take this edge in MST if it does
		// not forms a cycle
		int x_in = findIndex(x);
		int y_in = findIndex(y);
		if (find(x_in) != find(y_in)) {
			union1(x_in, y_in);
			ans += w;
			std::pair<int, std::string> temp;
			temp.first = w;
			temp.second = "(" + x + ", " + y + ")";
			returnObj.push_back(temp);
		}
	}
	return ans;
}
