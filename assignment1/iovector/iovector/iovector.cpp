#include <iostream>
#include <string>
#include <fstream>
#include "genlib.h"
#include "simpio.h"
#include "vector.h"

const int RANK = 10;
void printHisto(Vector<int> &histo);
void printStar(int count);

int main() {
	ifstream in;
	while(true) {
		cout << "Enter name: ";
		string filename = GetLine();
		in.open(filename.c_str());
		if (!in.fail()) break;
		in.clear();
		cout << "File open error." << endl;
	}
	
	Vector<int> histo;
	for (int i = 0; i < RANK; i++) {
		histo.add(0);
	}
	string line;
	while(true) {
		getline(in, line);
		if (in.fail()) break;
		int score = StringToInteger(line);
		histo[score / 10]++;
	}
	in.close();
	printHisto(histo);
	return 0;
}

void printHisto(Vector<int> &histo){
	for (int i = 0; i < RANK; i++) {
		int start = 10 * i;
		int end = 10 * (i + 1) -1;
		cout << start << "-" << end << ": ";
		printStar(histo[i]);
	}
}

void printStar(int count) {
	for(int i = 0; i < count; i++)
		cout << "X";
	cout << endl;
} 





