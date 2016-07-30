#include <iostream>
#include <string>
#include "error.h"
#include "genlib.h"
#include "simpio.h"

string censorString1(string text, string remove);
void censorString2(string &text, string remove);

/* int main() {
	string text, remove;
	cout << "Please enter text: ";
	text = GetLine();
	if (text == "a") error("no empty");
	cout << "Please enter remove: ";
	remove = GetLine();
	cout << censorString1(text, remove) << endl;
	censorString2(text, remove);
	cout << text << endl;
	return 0;
} */

string censorString1(string text, string remove) {
	for (int i = 0; i < remove.length(); i++) {
		int pos = 0;
		while((pos = text.find(remove[i], pos)) != string::npos) {
			text = text.substr(0, pos) + text.substr(pos + 1);
		}
	}
	return text;
}

void censorString2(string &text, string remove) {
	for (int i = 0; i < remove.length(); i++) {
		int pos = 0;
		while((pos = text.find(remove[i], pos)) != string::npos) {
			text.erase(pos, 1);
		}
	}
}