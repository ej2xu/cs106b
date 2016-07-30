#include <iostream>
#include "genlib.h"
#include "simpio.h"
#include "graph.h"

int main() {			
	Graph g;
	g.addNode("Atlanta");	
	g.addNode("Boston");
	g.addNode("Chicago");
	g.addNode("Dallas");
	g.addNode("Denver");
	g.addNode("Los Angeles");
	g.addNode("New York");
	g.addNode("Portland");
	g.addNode("San Francisco");
	g.addNode("Seattle");
	g.addUndirectedArc("Atlanta", "Chicago", 599);
	g.addUndirectedArc("Atlanta", "Dallas", 599);
	g.addUndirectedArc("Atlanta", "New York", 599);
	g.addUndirectedArc("Boston", "New York", 599);
	g.addUndirectedArc("Boston", "Seattle", 599);
	g.addUndirectedArc("Chicago", "Denver", 599);
	g.addUndirectedArc("Dallas", "Denver", 599);
	g.addUndirectedArc("Dallas", "Los Angeles", 599);
	g.addUndirectedArc("Dallas", "San Francisco", 599);
	g.addUndirectedArc("Denver", "San Francisco", 599);
	g.addUndirectedArc("Portland", "San Francisco", 599);
	g.addUndirectedArc("Portland", "Seattle", 599);

	g.printAdjList();
	
	return 0;
}