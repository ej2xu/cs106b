#include <iostream>
#include "genlib.h"
#include "lexicon.h"
#include "simpio.h"

string digitLetters(char ch) {
    switch (ch) {
        case '2': return ("abc");
        case '3': return ("def");
        case '4': return ("ghi");
        case '5': return ("jkl");
        case '6': return ("mno");
        case '7': return ("pqrs");
        case '8': return ("tuv");
        case '9': return ("wxyz");
        default: return ""; // skip '1', '0', and any non-integer
    }
}

void recCompletion(string prefix, Lexicon & lex) {
	if (lex.containsWord(prefix))
		cout << prefix << endl;
	for (char ch = 'a'; ch <= 'z'; ch++) {
		string nextPrefix = prefix + ch;
		if (lex.containsPrefix(nextPrefix))
			recCompletion(nextPrefix, lex);
	}
}

void recMnemonics(string prefix, string rest, Lexicon & lex) {
	if (rest.length() == 0) recCompletion(prefix, lex);
	else {
		string options = digitLetters(rest[0]);
		for (int i = 0; i < options.length(); i++)
			recMnemonics(prefix + options[i], rest.substr(1), lex);
	}
}
void ListCompletions(string digitSequence, Lexicon & lex) {
	recMnemonics("", digitSequence, lex); 
}

int main() {
	Lexicon lex("lexicon.dat");
	string digitSequence;
	while(true) {
		cout << "enter digit sequence between 2-9, type exit to exit program." << endl;
		digitSequence = GetLine();
		if (digitSequence == "exit") exit(0);
		else if (digitSequence != "") ListCompletions(digitSequence, lex);
	}
	return 0;
}