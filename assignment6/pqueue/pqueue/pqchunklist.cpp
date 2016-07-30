#include <iostream>
#include "genlib.h"
#include "pqueue.h"
/* 	struct Chunk {
 	  int *values;
	  int numUsed;  
 	  Chunk *next;
	};
	Chunk *head;
	Chunk *getNewChunk();
	void recEnqueue(Chunk *prev, Chunk *cur, int num);
	void insertChunk(Chunk *chunk, int num);
	void splitChunk(Chunk *chunk);
*/
const int MaxElemsPerBlock = 10;

PQueue::PQueue() {
	head = NULL;
}

PQueue::~PQueue() {
	while (head != NULL) {
		Chunk *next = head->next;
		delete[] head->values;
		delete head;
    	head = next;
    }
}

bool PQueue::isEmpty() {
    return (head == NULL);  
}

int PQueue::size() {
    int count = 0;
    for (Chunk *cur = head; cur != NULL; cur = cur->next)
		count += cur->numUsed;
    return count;
}

PQueue::Chunk *PQueue::getNewChunk() {
	Chunk *newOne = new Chunk;
	newOne->numUsed = 0;
	newOne->values = new int[MaxElemsPerBlock];
	return newOne;
}

void PQueue::insertChunk(Chunk *chunk, int num) {
	int index = chunk->numUsed - 1;
	while (num > chunk->values[index]) {
		chunk->values[index + 1] = chunk->values[index];
		index--;
	}
	chunk->values[index + 1] = num;
	chunk->numUsed++;
}

void PQueue::splitChunk(Chunk *chunk) {
	chunk->numUsed /= 2;
	Chunk *sHalf = new Chunk;
	sHalf->numUsed = MaxElemsPerBlock - chunk->numUsed;
	sHalf->values = new int[MaxElemsPerBlock];
	for (int i = 0; i < sHalf->numUsed; i++)
		sHalf->values[i] = chunk->values[chunk->numUsed + i];
	sHalf->next = chunk->next;
	chunk->next = sHalf;
}

void PQueue::recEnqueue(Chunk *prev, Chunk *cur, int num) {
	if (cur == NULL || num > cur->values[0]) {
		if (prev->numUsed < MaxElemsPerBlock) insertChunk(prev, num);
		else {
			splitChunk(prev);
			if (num > prev->next->values[0]) insertChunk(prev, num);
			else insertChunk(prev->next, num);
		}
	} else recEnqueue(cur, cur->next, num);
} 

void PQueue::enqueue(int newValue) {
	Chunk *cur = head, *prev = NULL; 
	if (cur == NULL || (newValue > cur->values[0] && cur->numUsed == MaxElemsPerBlock)) {
		Chunk *newOne = getNewChunk();
		newOne->values[0] = newValue;
		newOne->numUsed++;
		newOne->next = cur;
		head = newOne;
	} else if (newValue > cur->values[0] && cur->numUsed < MaxElemsPerBlock) {
		for (int i = cur->numUsed; i > 0; i--)
			cur->values[i] = cur->values[i - 1];
		cur->values[0] = newValue;
		cur->numUsed++;
	} else {
		recEnqueue(cur, cur->next, newValue);
	}
}

int PQueue::dequeueMax() {
	if (isEmpty())
		Error("Tried to dequeue max from an empty pqueue!");
	int value = head->values[0];
	if (head->numUsed > 1) {
		for (int i = 0; i < head->numUsed - 1; i++)
			head->values[i] = head->values[i+1];
		head->numUsed--;
	} else {
		delete[] head->values;
 		Chunk *toBeDeleted = head;
 		head = head->next;
		delete toBeDeleted;
	}
	return value;
}

int PQueue::bytesUsed() {
	int total = sizeof(*this);
	for (Chunk *cur = head; cur != NULL; cur = cur->next) {
 		total += sizeof(*cur);
		for (int i = 0; i < cur->numUsed; i++)
			total += sizeof(cur->values[i]);
	}
 	return total;
}
	

string PQueue::implementationName() {
	return "sorted chunklist";
}

void PQueue::printDebuggingInfo() {
   int count = 0;

	cout << "------------------ START DEBUG INFO ------------------" << endl;
	for (Chunk *cur = head; cur != NULL; cur = cur->next) {
		cout << "Chunk #" << count << " (at address " << cur << ") values = ";
		for (int i = 0; i < cur->numUsed; i++)
			cout << cur->values[i] << " ";
		cout << "number used: " << cur->numUsed;
        cout << " next = " << cur->next << endl;
		count++;
	}
	cout << "------------------ END DEBUG INFO ------------------" << endl;
}