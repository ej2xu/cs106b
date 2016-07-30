#include <iostream>
#include <cmath>
#include "genlib.h"
#include "simpio.h"
#include "vector.h"


void recCountVotes(Vector<int> & temp, int target, int diff, int index, int &counter) {
	if (index == temp.size()) {
		if (abs(diff) <= target)
			counter++;
		return;
	}
	recCountVotes(temp, target, diff + temp[index], index + 1, counter);
	recCountVotes(temp, target, diff - temp[index], index + 1, counter);
}

int CountCriticalVotes(Vector<int> & blocks, int blockIndex) {
	int blockVotes = blocks[blockIndex];
	Vector<int> temp = blocks;
	temp.removeAt(blockIndex);
	int criticalCount = 0;
	recCountVotes(temp, blockVotes, 0, 0, criticalCount);
	return criticalCount;
}


int main() {
	Vector<int> blocks;
    int blockIndex;
    while(true)
    {
        cout << "Enter vote blocks (enter 0 when finished): ";
        int num = GetInteger();
        if (num == 0) break;
        blocks.add(num);
    }
    while(true)
    {
        cout << "Enter block index (starting at 0): ";
        blockIndex = GetInteger();
        if (blockIndex < blocks.size()) break;
        else cout << "Invalid index" << endl;
    }
    int votes = CountCriticalVotes(blocks, blockIndex);
    cout << "Block index " << blockIndex << " has " << votes
        << " critical votes." <<endl;
    return 0;
}
