#include <iostream>
#include <string>
#include <cstdlib>
#include "genlib.h"
#include "error.h"
using namespace std;

void error(string msg) {
	cerr << msg << endl;
	exit(EXIT_FAILURE);
}