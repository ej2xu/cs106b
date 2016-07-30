#include <iostream>
#include "genlib.h"
#include "simpio.h"
#include "vector.h"
#include "Set.h"

const int SENTINEL = -1;

void recGetOptions(Vector<Set<int> > &optionMatrix, Vector<int> &requests, int remain, int index, Set<int> &option) {
	for (int i = index; i < requests.size(); i++) {
		if (remain >= requests[i]) {
			Set<int> temp = option;
			temp.add(i);
			optionMatrix.add(temp);
			recGetOptions(optionMatrix, requests, remain - requests[i], i + 1, temp);
		}
	}
	return;
}

void getOptions(Vector<Set<int> > &optionMatrix, Vector<int> &requests, int stockLength) {
	Set<int> option;
	recGetOptions(optionMatrix, requests, stockLength, 0, option);
}

void removeEntries(Vector<int> &temp, Set<int>::Iterator &it) {
	while (it.hasNext()) {
		temp[it.next()] = SENTINEL;
	}
	for (int i = temp.size() - 1; i >= 0 ; i--) {
		if (temp[i] == SENTINEL) temp.removeAt(i);
	}
}

int cutStock(Vector<int> & requests, int stockLength) {
	if (requests.size() == 0) return 0;
	Vector<Set<int> > optionMatrix;	
	int min = requests.size();
	getOptions(optionMatrix, requests, stockLength);
	for(int i = 0; i < optionMatrix.size(); i++) {
		Vector<int> temp = requests;
		Set<int>::Iterator it = optionMatrix[i].iterator();
		removeEntries(temp, it);
		int count = cutStock(temp, stockLength);
		count++;
		if (count < min) min = count;
	}
	return min;
}

/*	main function of testing copied from
	https://github.com/erickwill/Stanford-CS106B-Solutions/blob/master/Recursion/6_CutStock.cpp
	for saving time. */

int main () {
    cout << "Enter the length of a stock pipe: ";
    int stockLength = GetInteger();

    Vector<int> requests;
    while(true) {
        cout << "Enter requested pipe lengths (enter 0 when finished): ";
        int length = GetInteger();
        if (length == 0) break;
        requests.add(length);
    }

    int minPipes = cutStock(requests, stockLength);
    cout << "The minimum amount of pipes required = " << minPipes << endl;
    return 0;
}