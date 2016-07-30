/* File: main.cpp
 * --------------
 * Simple main module for PQueue assignment.
 */

#include <iostream>
#include "genlib.h"
#include "performance.h"
#include "pqueuetest.h"

/* void customTest() {
	PQueue<string> pq, copy;
	while (true) {
		cout << "press 1 to enqueue, 2 to dequeue, 0 to quit: ";
		int choice = GetInteger();
		if (choice == 0) break;
		else if (choice == 1) {
			while (true) {
				cout << "enter string to enqueue (enter to quit): ";
				string line = GetLine();
				if (line == "") {
					copy = pq;
					cout << "debuginfo of copy" << endl;
					copy.printDebuggingInfo();
					break;
				}
				pq.enqueue(line);
				pq.printDebuggingInfo();
			}
		}
		else if (choice == 2) {
			while (true) {
				cout << "enter to dequeue (\"exit\" to quit): ";
				string line = GetLine();
				if (line == "exit") break;
				pq.dequeueMax();
				pq.printDebuggingInfo();
			}
		}
	}
	cout << "debuginfo of copy" << endl;
	copy.printDebuggingInfo();
} */

int main()
{
	customTest();
//	BasicPQueueTest();
//	MorePQueueTest();
//	PQueueSortTest();
// 	PQueuePerformance();
    cout << endl << "Goodbye and have a nice day!" << endl;
    return (0);
}

