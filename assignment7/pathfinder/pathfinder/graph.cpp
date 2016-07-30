#include <iostream>
#include "graph.h"
#include "pqueue.h"

double getPathCost(Vector<Arc *> &path) {
	double cost = 0;
	for (int i = 0; i < path.size(); i++)
		cost += path[i]->cost;
	return cost;
}

int nodeCmp(Node *one, Node *two) {
	Set<Node *> oneNeigh, twoNeigh;
	Set<Arc *>::Iterator it = one->arcs.iterator();
	while (it.hasNext())
		oneNeigh.add(it.next()->finish);
	it = two->arcs.iterator();
	while (it.hasNext())
		twoNeigh.add(it.next()->finish);
	return oneNeigh.size() - twoNeigh.size();
}

int nodeSetCmp(Set<Node *> one, Set<Node *> two) {
	if (one.equals(two)) return 0;
	int adSumOne, adSumTwo;
	adSumOne = adSumTwo = 0;
	Set<Node *>::Iterator it = one.iterator();
	while (it.hasNext())
		adSumOne += int(it.next());
	it = two.iterator();
	while (it.hasNext())
		adSumTwo += int(it.next());
	return (adSumOne > adSumTwo ? 1 : -1);
}

int pathCmp(Vector<Arc *> one, Vector<Arc *> two) {
	double oneCost = getPathCost(one);
	double twoCost = getPathCost(two);
	if (twoCost == oneCost) return 0;
	return (twoCost > oneCost ? 1 : -1);
}

int arcCmp(Arc *one, Arc *two) {
	double oneCost = one->cost;
	double twoCost = two->cost;
	if (oneCost == twoCost) return 0;
	return (twoCost > oneCost ? 1 : -1);
}

Graph::Graph() {}

Graph::~Graph() {
	clear();
}

int Graph::size() {
	return nodes.size();
}

bool Graph::isEmpty() {
	return nodes.isEmpty();
}

void Graph::clear() {
	Set<Node *>::Iterator nitr = nodes.iterator();
	while (nitr.hasNext())
		delete nitr.next();
	Set<Arc *>::Iterator aitr = arcs.iterator();
	while (aitr.hasNext())
		delete aitr.next();
	nodes.clear();
	arcs.clear();
	nodeMap.clear();
}

Node *Graph::addNode(std::string name) {
	if (nodeMap.containsKey(name)) Error("addNode: Node " + name + " already exists");
	Node *node = new Node;
	node->name = name;
	return addNode(node);
}

Node *Graph::addNode(Node *node) {
	nodes.add(node);
	nodeMap[node->name] = node;
	return node;
}

Node *Graph::getNode(string name) {
	if (!nodeMap.containsKey(name)) Error("No node named " + name);
	return nodeMap[name];
}

void Graph::removeNode(string name) {
	if (nodeMap.containsKey(name))
		removeNode(nodeMap[name]);
}

void Graph::removeNode(Node *node) {
	Vector<Arc *> toRemove;
	Set<Arc *>::Iterator it = arcs.iterator();
	while (it.hasNext()) {
		Arc *arc = it.next();
		if (arc->start == node || arc->finish == node)
			toRemove.add(arc);
	}
	for (int i = 0; i < toRemove.size(); i++)
		removeArc(toRemove[i]);
	nodes.remove(node);
	nodeMap.remove(node->name);
	delete node;
}

Arc *Graph::addArc(string s1, string s2, double cost) {
	return addArc(getNode(s1), getNode(s2), cost);
}

Arc *Graph::addArc(Node *n1, Node *n2, double cost) {
	Arc *arc = new Arc;
	arc->start = n1;
	arc->finish = n2;
	arc->cost = cost;
	return addArc(arc);
}

Arc *Graph::addArc(Arc *arc) {
	arc->start->arcs.add(arc);
	arcs.add(arc);
	return arc;
}

void Graph::addUndirectedArc(string s1, string s2, double cost) {
	if (nodeMap.containsKey(s1) && nodeMap.containsKey(s2)) {
		addArc(nodeMap[s1], nodeMap[s2], cost);
		addArc(nodeMap[s2], nodeMap[s1], cost);
	}
}

void Graph::removeArc(string s1, string s2) {
	if (nodeMap.containsKey(s1) && nodeMap.containsKey(s2))
		removeArc(nodeMap[s1], nodeMap[s2]);
}

void Graph::removeArc(Node *n1, Node *n2) {
	Vector<Arc *> toRemove;
	Set<Arc *>::Iterator it = arcs.iterator();
	while (it.hasNext()) {
		Arc *arc = it.next();
		if (arc->start == n1 && arc->finish == n2)
			toRemove.add(arc);
	}
	for (int i = 0; i < toRemove.size(); i++)
		removeArc(toRemove[i]);
}

void Graph::removeUndirectedArc(string s1, string s2) {
	if (nodeMap.containsKey(s1) && nodeMap.containsKey(s2)) {
		removeArc(nodeMap[s1], nodeMap[s2]);
		removeArc(nodeMap[s2], nodeMap[s1]);
	}
}

Set<Node *> Graph::getNeighbors(string name) {
	return getNeighbors(getNode(name));
}

Set<Node *> Graph::getNeighbors(Node *node) {
	Set<Node *> result;
	Set<Arc *>::Iterator it = node->arcs.iterator();
	while (it.hasNext())
		result.add(it.next()->finish);
	return result;
}

Set<Node *> Graph::findSmallDomSet() {
	PQueue<Node *> queue(nodeCmp);
	Set<Node *> result, dominated;
	Set<Node *>::Iterator it = nodes.iterator();
	while (it.hasNext())		
		queue.enqueue(it.next());
	while (dominated.size() < nodes.size()) {
		Node *curr = queue.dequeueMax();
		Set<Node *> neighbors = getNeighbors(curr);
		if (!dominated.contains(curr) || !neighbors.isSubsetOf(dominated)) {
			result.add(curr);
			dominated.unionWith(neighbors);
			dominated.add(curr);
		}
	}
	return result;
}

Set<Node *> findSet(Node *node, Set<Set<Node *> > &comps) {
	Set<Set<Node *> >::Iterator it = comps.iterator();
	while (it.hasNext()) {
		Set<Node *> comp = it.next();
		if (comp.contains(node)) return comp;
	}
	Error("No component contains this node.");
}

Vector<Arc *> Graph::findMST() {
	PQueue<Arc *> queue(arcCmp);
	Vector<Arc *> MST;
	Set<Set<Node *> > comps(nodeSetCmp);
	Set<Node *>::Iterator nodeIt = nodes.iterator();
	while (nodeIt.hasNext()) {		
		Set<Node *> comp;
		comp.add(nodeIt.next());
		comps.add(comp);
	}
	Set<Arc *>::Iterator arcIt = arcs.iterator();
	while (arcIt.hasNext())
		queue.enqueue(arcIt.next());
	while (!queue.isEmpty() && comps.size() > 1) {
		Arc *arc = queue.dequeueMax();
		Set<Node *> comp1 = findSet(arc->start, comps);
		Set<Node *> comp2 = findSet(arc->finish, comps);
		if(!comp1.equals(comp2)) {
			MST.add(arc);
			comps.remove(comp1);
			comps.remove(comp2);
			comp1.unionWith(comp2);
			comps.add(comp1);
		}
	}
	return MST;
}

Vector<Arc *> Graph::findShortestPath(string s1, string s2, int &count) {
	return findShortestPath(getNode(s1), getNode(s2), count);
}

Vector<Arc *> Graph::findShortestPath(Node *start, Node *finish, int &count) {
	Vector<Arc *> path;
	PQueue<Vector<Arc *> > queue(pathCmp);
	Set<Node *> fixed;
	while (start != finish) {
		if (!fixed.contains(start)) {
			fixed.add(start);
			Set<Arc *>::Iterator it = (start->arcs).iterator();
			while (it.hasNext()) {
				Arc *arc = it.next();
				if (!fixed.contains(arc->finish)) {
					path.add(arc);
					queue.enqueue(path);
					path.removeAt(path.size() - 1);
				}
			}
		}
		if (queue.isEmpty()) {
			path.clear();
			return path;
		}
		path = queue.dequeueMax();
		start = path[path.size() - 1]->finish;
		count++;
	}
	return path;
}

void Graph::removeArc(Arc *arc) {
	arc->start->arcs.remove(arc);
	arcs.remove(arc);
	delete arc;
}

Set<Arc *> &Graph::getArcSet() {
	return arcs;
}

void Graph::printAdjList() {
	Set<Node *>::Iterator nitr = nodes.iterator();
	while (nitr.hasNext()) {
		Node *node = nitr.next();
		cout << node->name << " -> ";
		bool first = true;
		Set<Arc *>::Iterator aitr = (node->arcs).iterator();
		while (aitr.hasNext()) {
			if (!first) cout << ", ";
			cout << (aitr.next())->finish->name;
			first = false;
		}
		cout << endl;
	}
}