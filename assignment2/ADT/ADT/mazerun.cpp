#include <iostream>
#include "mazerun.h"
#include "maze.h"
#include "genlib.h"
#include "queue.h"
#include "stack.h"
#include "grid.h"
#include "vector.h"
#include "random.h"
#include "set.h"

const int TOTAL_ROWS = 10;
const int TOTAL_COLUMNS = 20;

void generateMaze(Maze &maze);
bool isAllIncluded(Grid<bool> &ex);
pointT chooseNeighbor(Maze &maze, pointT &currentPt);
Stack<pointT> solveMaze(Maze &maze);
void setUp(pointT &startPt, pointT &goalPt);
bool endAtGoal(Stack<pointT> &path, pointT &goalPt);
void extendPathCollection(Queue<Stack<pointT> > &pathCollection, Maze &maze, Stack<pointT> &path, Set<pointT> &ptExplored);
void drawPath(Stack<pointT> &solPath, Maze &maze);

int comparePt(pointT one, pointT two) {
	int oneSum = one.row * TOTAL_COLUMNS + one.col;
	int twoSum = two.row * TOTAL_COLUMNS + two.col;
	if (oneSum == twoSum) return 0;
	else if (oneSum > twoSum) return 1;
	else return -1;
}

void mazeRun() {
	Randomize();
	Maze maze(TOTAL_ROWS, TOTAL_COLUMNS, true);
	maze.draw();
	generateMaze(maze);

    Stack<pointT> solPath = solveMaze(maze);
	drawPath(solPath, maze);
}

void generateMaze(Maze &maze) {
	Grid<bool> excluded(TOTAL_ROWS, TOTAL_COLUMNS);
	for (int row = 0; row < excluded.numRows(); row++)
		for (int col = 0; col < excluded.numCols(); col++)
			excluded(row, col) = true;

	pointT currentPt, neighbor;
	currentPt.row = RandomInteger(0, TOTAL_ROWS - 1);
    currentPt.col = RandomInteger(0, TOTAL_COLUMNS - 1);
	excluded(currentPt.row, currentPt.col) = false;

	while(!isAllIncluded(excluded)) {
		neighbor = chooseNeighbor(maze, currentPt);
		if(excluded(neighbor.row, neighbor.col)) {
			maze.setWall(currentPt, neighbor, false);
			excluded(neighbor.row, neighbor.col) = false;
		}
		currentPt = neighbor;
	}	
}

bool isAllIncluded(Grid<bool> &ex) {
	for (int row = 0; row < ex.numRows(); row++)
		for (int col = 0; col < ex.numCols(); col++)
			if (ex(row, col)) return false;
	return true;
}

pointT chooseNeighbor(Maze &maze, pointT &pt) {
	Vector<pointT> neighbors;
	pointT ptWest, ptEast, ptNorth, ptSouth;
	ptWest.row = pt.row;
	ptWest.col = pt.col - 1;
	ptEast.row = pt.row;
	ptEast.col = pt.col + 1;
	ptNorth.row = pt.row + 1;
	ptNorth.col = pt.col;
	ptSouth.row = pt.row - 1;
	ptSouth.col = pt.col;
	if (maze.pointInBounds(ptWest))
		neighbors.add(ptWest);
	if (maze.pointInBounds(ptEast))
		neighbors.add(ptEast);
	if (maze.pointInBounds(ptNorth))
		neighbors.add(ptNorth);
	if (maze.pointInBounds(ptSouth))
		neighbors.add(ptSouth);
	int size = neighbors.size();
	return neighbors[RandomInteger(0,size - 1)];
}

Stack<pointT> solveMaze(Maze &maze) {
	pointT startPt, goalPt;
	setUp(startPt, goalPt);	
	Queue<Stack<pointT> > pathCollection;
	Set<pointT> ptExplored(comparePt);
	Stack<pointT> initPath, path;
	initPath.push(startPt);
	ptExplored.add(startPt);
	pathCollection.enqueue(initPath);

	while(!pathCollection.isEmpty()) {
		path = pathCollection.dequeue();
		if (endAtGoal(path, goalPt)) break;
		extendPathCollection(pathCollection, maze, path, ptExplored);
	}
	return path;
}

void setUp(pointT &startPt, pointT &goalPt) {
	startPt.row = 0;
    startPt.col = 0;
    goalPt.row = TOTAL_ROWS - 1;
    goalPt.col = TOTAL_COLUMNS - 1;
}

bool endAtGoal(Stack<pointT> &path, pointT &goalPt) {
	pointT pt = path.peek();
	return ((pt.row == goalPt.row) && (pt.col == goalPt.col));
}

void extendPathCollection(Queue<Stack<pointT> > &pathCollection, Maze &maze, Stack<pointT> &path, Set<pointT> &ptExplored) {
	pointT pt = path.peek();
	pointT neighbor;
	for (int i = 0; i < 4; i++) {
        neighbor = pt;
        switch(i) {
            case 0: 
                neighbor.col++;
                break;
            case 1: 
                neighbor.col--;
                break;
            case 2: 
                neighbor.row--;
                break;
            case 3: 
                neighbor.row++;
				break;
			default: ;
        }
		if (!ptExplored.contains(neighbor) && maze.pointInBounds(neighbor) && !maze.isWall(neighbor, pt)) {
			Stack<pointT> nextPath = path;			
			nextPath.push(neighbor);
			pathCollection.enqueue(nextPath);
			ptExplored.add(neighbor);
			
		}
	}
}

void drawPath(Stack<pointT> &solPath, Maze &maze) {
	Stack<pointT> path;
	while(!solPath.isEmpty()) {
		path.push(solPath.pop());
	}
	while(!path.isEmpty()) {
		maze.drawMark(path.pop(), "Blue");
	}
}