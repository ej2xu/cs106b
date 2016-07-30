/* pathfinder.cpp
 * ---------------
 * A starter file with some starting constants and handy utility routines.
 */
#include <iostream>
#include <fstream>
#include <cmath> 
#include "genlib.h"
#include "simpio.h"
#include "extgraph.h"
#include "graph.h"
#include "map.h"
#include "set.h"

const double CircleRadius =.05;     	// the radius of a node
const int LabelFontSize = 9;          // for node name labels

struct coordT {
	double x, y;
};


/* Function: DrawFilledCircleWithLabel
 * Usage:  DrawFilledCircleWithLabel(center, "Green", "You are here");
 * -------------------------------------------------------------------
 * Uses facilities from extgraph to draw a circle filled with
 * color specified. The circle is centered at the given coord has the
 * specified radius.  A label is drawn to the right of the circle.
 * You can leave off the last argument if no label is desired.
 */
void DrawFilledCircleWithLabel(coordT center, string color, string label = "")
{
	MovePen(center.x + CircleRadius, center.y);
	SetPenColor(color);
	StartFilledRegion(1.0);
	DrawArc(CircleRadius, 0, 360);
	EndFilledRegion();
	if (!label.empty()) {
		MovePen(center.x + CircleRadius, center.y);
		SetFont("Helvetica");
		SetPointSize(LabelFontSize);
		DrawTextString(label);
	}
}

/* Function: DrawLineBetween
 * Usage:  DrawLineBetween(coord1, coord2, "Black");
 * -------------------------------------------------
 * Uses facilities from extgraph to draw a line of the
 * specified color between the two given coordinates.
 */
void DrawLineBetween(coordT start, coordT end, string color)
{
	SetPenColor(color);
	MovePen(start.x, start.y);
	DrawLine(end.x - start.x, end.y - start.y);
}


/* Function: GetMouseClick
 * Usage:  loc = GetMouseClick();
 * ------------------------------
 * Waits for the user to click somewhere on the graphics window
 * and returns the coordinate of where the click took place.
 */
coordT GetMouseClick()
{
	coordT where;
	WaitForMouseDown();
	WaitForMouseUp();
	where.x = GetMouseX();
	where.y = GetMouseY();
	return where;
}

/* Function: WithinDistance
 * Usage:  if (WithinDistance(click, pt))...
 * -----------------------------------------
 * Returns true if the Cartesian distance between the two coordinates
 * is <= the specified distance, false otherwise. If not specified,
 * the distance is assumed to be size of the node diameter on screen.
 */
bool WithinDistance(coordT pt1, coordT pt2, double maxDistance = CircleRadius*2)
{
	double dx = pt2.x - pt1.x;
	double dy = pt2.y - pt1.y;
	double distance = sqrt(dx*dx + dy*dy);
	return (distance <= maxDistance);
}

void promptUserForFile(ifstream &in) {
	while (true) {
		cout << "\nPlease enter name of graph data file: ";
		string filename = GetLine();
		in.open(filename.c_str());
		if (!in.fail()) break;
		in.clear();
		cout << "Unable to open file named \"" + filename + "\". Please try again." << endl; 
	}
}

string drawBackGround(ifstream &in) {
	string image;
	in >> image;
	DrawNamedPicture(image);
	return image;
}

void readNodes(ifstream &in, Graph &graph, Map<coordT> &m) {
	string name;
	coordT location;	
	while (true) {
		in >> name;
		if (name == "ARCS") break;
		graph.addNode(name);
		in >> location.x;
		in >> location.y;
		m[name] = location;
	}
}

void readArcs(ifstream &in, Graph &graph) {
	string s1, s2;
	double cost;
	while (in >> s1) {		
		in >> s2;
		in >> cost;
		graph.addUndirectedArc(s1, s2, cost);
	}
}

string loadGraphFile(Graph &graph, Map<coordT> &m) {
	ifstream in;
	promptUserForFile(in);
	InitGraphics();
	string image = drawBackGround(in);
	string token;
	in >> token;
	if (token == "NODES") readNodes(in, graph, m);
	else Error("File format error."); 
	readArcs(in, graph);
	in.close();
	return image;
}

void drawVertices(Map<coordT> &m) {
	Map<coordT>::Iterator locIt = m.iterator();
	while (locIt.hasNext()) {
		string name = locIt.next();
		DrawFilledCircleWithLabel(m[name], "Black", name);
	}
}

void drawEdges(Graph &graph, Map<coordT> &m) {
	Set<Arc *>::Iterator edgeIt = graph.getArcSet().iterator();
	while (edgeIt.hasNext()) {
		Arc *arc = edgeIt.next();
		DrawLineBetween(m[arc->start->name], m[arc->finish->name], "Black");
	}
}

void drawMap(Graph &graph, Map<coordT> &m) {
	drawVertices(m);
	drawEdges(graph, m);
}

int promptForChoice() {
	while (true) {
        cout << "\nYour options are: " << endl;
        cout << "    (1) Choose a new graph file" << endl <<
            "    (2) Find shortest path using Dijkstra's algorithm" << endl <<
            "    (3) Compute the minimal spanning tree using Kruskal's algorithm" << endl <<
			"    (4) Display a small dominated vertex set" << endl <<
            "    (5) Quit" << endl << "Enter Selection: ";
        int choice = GetInteger();
        if (choice >= 1 && choice <= 5) return choice;
        cout << "Invalid choice. Try again." << endl;
    }
}

string getLocName(string s, Map<coordT> &m) {
	string name;
	while (true) {
		cout << s;
		coordT loc = GetMouseClick();
		Map<coordT>::Iterator it = m.iterator();
		while (it.hasNext()) {
			name = it.next();
			if (WithinDistance(loc, m[name])) {
				cout << name << " chosen." <<endl;
				DrawFilledCircleWithLabel(m[name], "Red", name);
				return name;
			}
		}
		cout << "Invalid click. Please try again." << endl << endl;
	}
}

void displayMinResult(string &s1, string &s2, double &cost, int &dequeueCount) {
	cout << "Done! The shortest path from " << s1 << " to " << s2 << " is " << cost	<< " miles." << endl
		<< "The algorithm dequeued " << dequeueCount << " paths to find the optimal one." << endl
		<< "Hit return to continue: ";
	GetLine();
}

void displayMinPath(Graph &graph, Map<coordT> &m, string &s1, string &s2) {
	cout << "\nFinding shortest path using Dijkstra...  " << endl;
	int dequeueCount = 0;
	Vector<Arc *> path = graph.findShortestPath(s1, s2, dequeueCount);
	string prev = s1, curr;
	for (int i = 1; i < path.size(); i++) {
		curr = path[i]->start->name;
		DrawLineBetween(m[prev], m[curr], "Red");
		DrawFilledCircleWithLabel(m[curr], "Red", curr);
		prev = curr;
	}
	DrawLineBetween(m[prev], m[s2], "Red");
	double cost = getPathCost(path);
	displayMinResult(s1, s2, cost, dequeueCount);	
	prev = s1;
	for (int i = 0; i < path.size(); i++) {
		curr = path[i]->finish->name;
		DrawLineBetween(m[prev], m[curr], "Black");
		DrawFilledCircleWithLabel(m[prev], "Black", prev);
		prev = curr;
	}
	DrawFilledCircleWithLabel(m[prev], "Black", prev);
}

void displayMST(Graph &graph, Map<coordT> &m) {
	Vector<Arc *> MST = graph.findMST();
	double netLength = 0;
	for (int i = 0; i < MST.size(); i++) {
		Arc *arc = MST[i];
		netLength += arc->cost;
		DrawLineBetween(m[arc->start->name], m[arc->finish->name], "Red");
	//  Pause(0.05);
	}
	cout << endl << "MST now displayed." << endl
		<< "Total network length is " << netLength << " miles." << endl
		<< "Hit return to continue: ";
	GetLine();
	drawEdges(graph, m);
}

void displayDomSet(Graph &graph, Map<coordT> &m) {
	Set<Node *> domSet = graph.findSmallDomSet();
	Set<Node *>::Iterator it = domSet.iterator();
	while (it.hasNext()) {
		string name = it.next()->name;
		DrawFilledCircleWithLabel(m[name], "Red", name);
	}
	cout << endl << "Displaying a small dominated set, hit return to continue: ";
	GetLine();
	it = domSet.iterator();
	while (it.hasNext()) {
		string name = it.next()->name;
		DrawFilledCircleWithLabel(m[name], "Black", name);
	}
}

int main() {
	InitGraphics();
	SetWindowTitle("CS106 Pathfinder");
    cout << "This masterful piece of work is a graph extravaganza!" << endl
        << "The main attractions include a lovely visual presentation of the graph" << endl
        << "along with an implementation of Dijkstra's shortest path algorithm and" << endl
        << "the computation of a minimal spanning tree.  Enjoy!" << endl;
	Graph graph;
	Map<coordT> nodeCor;
	string loc1, loc2, image;
	image = loadGraphFile(graph, nodeCor);
	drawMap(graph, nodeCor);
	while (true) {
		int choice = promptForChoice();
		switch (choice) {
			case 1:
				graph.clear();
				nodeCor.clear();
				image = loadGraphFile(graph, nodeCor);
				drawMap(graph, nodeCor);
				break;
			case 2:
				cout << endl;
				loc1 = getLocName("Click on starting location...  ", nodeCor);
				loc2 = getLocName("Click on ending location...  ", nodeCor);
				displayMinPath(graph, nodeCor, loc1, loc2);
				break;
			case 3:
				InitGraphics();
				DrawNamedPicture(image);
				drawVertices(nodeCor);
				displayMST(graph, nodeCor);
				break;
			case 4:
				displayDomSet(graph, nodeCor);
				break;
			case 5: exit(0);
		}
	} 

    return (0);
}