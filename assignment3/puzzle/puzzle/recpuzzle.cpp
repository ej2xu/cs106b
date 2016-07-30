#include <iostream>
#include "genlib.h"
#include "simpio.h"
#include "vector.h"
#include "set.h"

bool recSolvable(int start, Vector<int> & squares, Set<int> &indexVisited) {
	if (start < 0 || start >= squares.size() || indexVisited.contains(start)) return false;
	if (start == (squares.size() - 1)) return true;
	indexVisited.add(start);
	return (recSolvable(start + squares[start], squares, indexVisited)
		||recSolvable(start - squares[start], squares, indexVisited));
}

bool Solvable(int start, Vector<int> & squares) {
	Set<int> indexVisited;
	return recSolvable(start, squares, indexVisited);
}

/* All tests in main are copied from
https://github.com/erickwill/Stanford-CS
106B-Solutions/blob/master/Recursion/ to
save work */

int main () {
    Vector<int> squares;
    int start;

    while(true) {
        cout << "Enter integers for puzzle (enter 0 when done): ";
        int num = GetInteger();
        squares.add(num);
        if (num == 0) break;
    }
    while(true) {
        cout << "Enter starting index: ";
        start = GetInteger();
        if (start < squares.size() && start >= 0) break;
    }
    
    if (Solvable(start, squares))
        cout << "True! The puzzle is solvable." << endl;
    else
        cout << "False! The puzzle cannot be solved." << endl;

    return 0;
}