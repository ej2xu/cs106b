#include <iostream>
#include <fstream>
#include <string>
#include "genlib.h"
#include "random.h"
#include "simpio.h"
#include "randomwriter.h"
#include "map.h"
#include "vector.h"

void parseText(ifstream &in, int order, Map<Vector<char> > &seedMap);
string getInitial(Map<Vector<char> > &seedMap);
void generateRandText(int order, string seed, Map<Vector<char> > &seedMap);

const int MAX_CHARS = 2000;

void randWriter() {
	Randomize();
	ifstream in;
	int order = 0;
	Map<Vector<char> > seedMap;
	while(true) {
		cout << "Please enter filename containing source text: ";
		string filename = GetLine();
		in.open(filename.c_str());
		if (!in.fail()) break;
		in.clear();
		cout << "Could not open file named \"" <<  filename << "\". Please try again." << endl;
	}
	
	while(true) {
		cout << "What order of analysis? (a number from 1 to 10): ";
		order = GetInteger();
		if (order >=1 && order <= 10) break;
		cout << "Invalid number" << endl;
	}
	parseText(in, order, seedMap);
	in.close();

	string seed = getInitial(seedMap);
	generateRandText(order, seed, seedMap);
	seedMap.clear();
}

void parseText(ifstream &in, int order, Map<Vector<char> > &seedMap) {
	cout << "Analysing input text..." << endl << endl;
	string seed;
	int nextChar;
	while((nextChar = in.get()) != EOF) {
		if (seed.length() == order) {
			seedMap[seed].add(nextChar);
			seed = seed.substr(1);
		}
		seed += nextChar;
	}
} 

string getInitial(Map<Vector<char> > &seedMap) {
	string maxSeed;
	int maxFreq = -1;
	Map<Vector<char> >::Iterator it = seedMap.iterator();
	while(it.hasNext()) {
		string seed = it.next();
		int frequency = seedMap[seed].size();
		if (frequency > maxFreq) {
			maxFreq = frequency;
			maxSeed = seed;
		}
	}
	return maxSeed;
}

void generateRandText(int order, string seed, Map<Vector<char> > &seedMap) {
	cout << seed;
	int counter = order;
	while (seedMap.containsKey(seed) && counter < MAX_CHARS) {
		int index = RandomInteger(0, seedMap[seed].size() - 1);
		char nextChar = seedMap[seed].getAt(index);
		cout << nextChar;
		counter++;
		seed = seed.substr(1) + nextChar;
	}
	cout << endl << endl;
}
