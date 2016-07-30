#include "genlib.h"
#include "simpio.h"
#include "randomwriter.h"
#include "mazerun.h"
#include <iostream>

 int main() {
	while(true) {
		cout << "Here is the menu" << endl;
		cout << "0. Exit" << endl;
        cout << "1. Random Writer" << endl;
        cout << "2. Maze" << endl;
        cout << endl << "Choice: ";
		int choice = GetInteger();
		if (choice == 0) exit(0);
		else if (choice == 1) randWriter();
		else if (choice == 2) mazeRun();
	}
	return 0;
} 