#ifndef _graph_h
#define _graph_h

#include "genlib.h"
#include "set.h"
#include "vector.h"
#include "map.h"

struct Arc;

struct Node {
	string name;
	Set<Arc *> arcs;
};

struct Arc {
	Node *start;
	Node *finish;
	double cost;
};

double getPathCost(Vector<Arc *> &path);

class Graph {

	public:
		Graph();

		~Graph();

		int size();

		bool isEmpty();

		void clear();

		Node *addNode(string name);
		Node *addNode(Node *node);

		void removeNode(string name);
		void removeNode(Node *node);

		Node *getNode(string name);

		Arc *addArc(string s1, string s2, double cost);
		Arc *addArc(Node *n1, Node *n2, double cost);
		Arc *addArc(Arc *arc);

		void removeArc(string s1, string s2);
		void removeArc(Node *n1, Node *n2);
		void removeArc(Arc *arc);
		Vector<Arc *> findShortestPath(string s1, string s2, int &count); //Dijkstra's Algorithm
		Vector<Arc *> findShortestPath(Node *start, Node *finish, int &count);
		Vector<Arc *> findMST(); //Kruskal’s Algorithm
		Set<Node *> findSmallDomSet(); // Greedy approximation

		Set<Arc *> & getArcSet();

		Set<Node *> getNeighbors(string name);
		Set<Node *> getNeighbors(Node *node);
		void addUndirectedArc(string s1, string s2, double cost);
		void removeUndirectedArc(string s1, string s2);

		void printAdjList();
	
private:
	Graph(const Graph & src) {}
	const Graph & operator=(const Graph & src) {return *this; }

	Set<Node *> nodes;
	Set<Arc *> arcs;
	Map<Node *> nodeMap;
	
};

#endif