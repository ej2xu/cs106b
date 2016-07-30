#include "genlib.h"
#include "simpio.h"
#include <iostream>
#include <string>
#include "strutils.h"
#include "extgraph.h"
#include "random.h"
#include "gboggle.h"
#include "lexicon.h"
#include "grid.h"
#include "set.h"

const int CUBE_FACE = 6, MIN_WORD_LENGTH = 4, STANDARD_SIZE = 4, BIG_SIZE = 5;

string StandardCubes[16]  = 
{"AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS", "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
 "DISTTY", "EEGHNW", "EEINSU", "EHRTVW", "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"};
 
string BigBoggleCubes[25]  = 
{"AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM", "AEEGMU", "AEGMNN", "AFIRSY", 
"BJKQXZ", "CCNSTW", "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT", "DHHLOR", 
"DHLNOR", "EIIITT", "EMOTTT", "ENSSSU", "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"};

void GiveInstructions()
{
    cout << endl << "The boggle board is a grid onto which I will randomly distribute " 
	 << "cubes. These 6-sided cubes have letters rather than numbers on the faces, " 
	 << "creating a grid of letters on which you try to form words. You go first, " 
	 << "entering all the words you can find that are formed by tracing adjoining " 
	 << "letters. Two letters adjoin if they are next to each other horizontally, " 
	 << "vertically, or diagonally. A letter can only be used once in the word. Words "
	 << "must be at least 4 letters long and can only be counted once. You score points "
	 << "based on word length: a 4-letter word is worth 1 point, 5-letters earn 2 "
	 << "points, and so on. After your puny brain is exhausted, I, the super computer, "
	 << "will find all the remaining words and double or triple your paltry score." << endl;
	
    cout << "\nHit return when you're ready...";
    GetLine();
}

static void Welcome()
{
    cout << "Welcome!  You're about to play an intense game of mind-numbing Boggle. " 
	 << "The good news is that you might improve your vocabulary a bit.  The "
	 << "bad news is that you're probably going to lose miserably to this little "
	 << "dictionary-toting hunk of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

void swap (char &x, char &y) {
	char temp = x;
	x = y;
	y = temp;
}

bool legalWord(Lexicon &lex, Set<string> &wordSet, string word) {
	return (word.length() >= MIN_WORD_LENGTH && lex.containsWord(word) && !wordSet.contains(word));
}

void displayBoard(Grid<char> &board, int size) {
	for (int row = 0; row < size; row++)
		for (int col = 0; col < size; col++)
			LabelCube(row, col, board(row, col));
}

string userInput(int size) {
	while(true) {
        cout << "Enter letters for board. Exactly " << size * size << " letters required: ";
        string letters = GetLine();
		if (letters.length() >= size * size) {
			letters = ConvertToUpperCase(letters);
			return letters;
		}
        cout << "Incorrect amount of letters." << endl;
    }
}

bool forceConfiguration() {
	while(true) {
		cout << "Force board configuration? (Y/N): ";
        string answer = GetLine();
		if (answer == "Y") return true;
		if (answer == "N") return false;
		cout << "Please enter a valid answer, watch for case sensitivity." << endl;
	}
}

string stringArray2letters(string arr[], int size) {
	int arrLength = size * size;
	string result;
	for (int i = 0; i < arrLength; i++) {
		int index = RandomInteger(0, CUBE_FACE - 1);
		result += arr[i].at(index);
	}
	return result;
}

void fillBoard(string &letters, Grid<char> &board, int size, bool flag) {
	int count = 0;
	for (int row = 0; row < size; row++)
		for (int col = 0; col < size; col++)
			board(row, col) = letters[count++];
	if (!flag) {
		for (int row = 0; row < size; row++)
			for (int col = 0; col < size; col++) {
				int randomRow = RandomInteger(row, size - 1);
				int randomCol = RandomInteger(col, size - 1);
				swap(board(row, col), board(randomRow, randomCol));
			}
	}	
}

void initBoardLetters(Grid<char> &board, int size) {
	string letters;
	bool forceConfig = forceConfiguration();
	if(forceConfig)
		letters = userInput(size);
	else {
		if (size == STANDARD_SIZE)
			letters = stringArray2letters(StandardCubes, size);
		else letters = stringArray2letters(BigBoggleCubes, size);
	}
	fillBoard(letters, board, size, forceConfig);	
}

int getSize() {
	while(true) {
	cout << "1. Standard Board (4x4)" << endl;
    cout << "2. Big Board (5x5)" << endl;
	cout << "Select Board: ";
    int choice = GetInteger();
	if (choice == 1) return STANDARD_SIZE;
	if (choice == 2) return BIG_SIZE;
	cout << "Invalid choice! Please enter nunber 1 or 2." << endl;
	}
}

void getBoard(Grid<char> &board) {
	int boardSize = getSize();
	board.resize(boardSize, boardSize);
	DrawBoard(boardSize, boardSize);
	initBoardLetters(board, boardSize);
	displayBoard(board, boardSize);
}

bool isOffBoard(int row, int col, int numRows, int numCols) {
	return row < 0 || col < 0 || row >= numRows || col >= numCols;
}
bool legalCube(Grid<bool> &usedCubes, Grid<char> &board, int size, int row, int col) {
	return !isOffBoard(row, col, size, size) && !usedCubes(row, col);
}

void highLight(int row, int col) {
	HighlightCube(row, col, true);
	Pause(0.5);
	HighlightCube(row, col, false);
}

bool recWordOnBoard(Grid<char> &board, Grid<bool> &usedCubes, int size, string rest, int row, int col) {
	if (rest.empty()) return true;
	int nextRow, nextCol;
	for (int i = 0; i < 9; i++) {
		nextRow = row - 1 + i / 3;
		nextCol = col - 1 + i % 3;
		if (legalCube(usedCubes, board, size, nextRow, nextCol) && board(nextRow, nextCol) == rest[0]) {
			usedCubes(nextRow, nextCol) = true;
			if (recWordOnBoard(board, usedCubes, size, rest.substr(1), nextRow, nextCol)) {
				highLight(nextRow, nextCol);
				return true;
			}
			usedCubes(nextRow, nextCol) = false;
		}
	}
	return false;
}

void init(Grid<bool> &usedCubes, int size) {
	for(int row = 0; row < size; row++)
		for(int col = 0; col < size; col++)
			usedCubes(row, col) = false;
}

bool wordOnBoard(Grid<char> &board, string guess) {
	int size = board.numCols();
	Grid<bool> usedCubes(size, size);
	init(usedCubes, size);
	for(int row = 0; row < size; row++)
		for(int col = 0; col < size; col++)
			if (board(row, col) == guess[0]) {
				usedCubes(row, col) = true;
				if (recWordOnBoard(board, usedCubes, size, guess.substr(1), row, col)) {
					highLight(row, col);
					return true;
				}
				usedCubes(row, col) = false;
			}
	return false;
}

void parseGuess(Set<string> &foundWords, Grid<char> &board, Lexicon &lex, string guess) {
	playerT human = Human;
	if (!legalWord(lex, foundWords, guess)) {
		cout << "Either you've already guessed that word or not a valid word!" << endl;
        return;
	}
	if (wordOnBoard(board, guess)) {
		foundWords.add(guess);
		RecordWordForPlayer(guess, human);
	} else {
		cout << "Word isn't on board." << endl;
	}
}

void playerTurn(Set<string> &foundWords, Grid<char> &board, Lexicon &lex) {	
	while(true) {
		cout << endl << "Enter a word: ";
		string guess = GetLine();
		guess = ConvertToUpperCase(guess);
		if (guess == "") break;
		parseGuess(foundWords, board, lex, guess);
	}
}

void recFindWord(Set<string> &foundWords, Lexicon &lex, Grid<char> &board, Grid<bool> &usedCubes, int size, int row, int col, string prefix, playerT &pc) {
	if (legalWord(lex, foundWords, prefix)) {
		foundWords.add(prefix);
		RecordWordForPlayer(prefix, pc); 
	}
	int nextRow, nextCol;
	for (int i = 0; i < 9; i++) {
		nextRow = row - 1 + i / 3;
		nextCol = col - 1 + i % 3;
		if (legalCube(usedCubes, board, size, nextRow, nextCol)) {
			string nextPrefix = prefix + board(nextRow, nextCol);
			if (lex.containsPrefix(nextPrefix)) {
				usedCubes(nextRow, nextCol) = true;
				recFindWord(foundWords, lex, board, usedCubes, size, nextRow, nextCol, nextPrefix, pc);
				usedCubes(nextRow, nextCol) = false;
			}
		}
	}
}

void pcTurn(Set<string> &foundWords, Grid<char> &board, Lexicon &lex) {
	playerT pc = Computer;
	int size = board.numCols();
	Grid<bool> usedCubes(size, size);
	init(usedCubes, size);
	for(int row = 0; row < size; row++)
		for(int col = 0; col < size; col++) {
			string prefix;
			prefix += board(row, col);
			usedCubes(row, col) = true;
			recFindWord(foundWords, lex, board, usedCubes, size, row, col, prefix, pc);
			usedCubes(row, col) = false;
		}
}

void setUp(Lexicon &lex) {
	Grid<char> board;
	getBoard(board);
	Set<string> foundWords;
	playerTurn(foundWords, board, lex);
	pcTurn(foundWords, board, lex);
}

bool playAgain() {
	while(true) {
		cout << endl << "Play again? (Y/N): ";
		string answer = GetLine();
		if (answer == "Y") return true;
		if (answer == "N") return false;
		cout << "Please enter a valid answer, watch for case sensitivity." << endl;
	}
}

int main() {
	Randomize();
	SetWindowSize(9, 5);
	Lexicon lex("lexicon.dat");
	while(true) {
		InitGraphics();
		Welcome();
		GiveInstructions();
		setUp(lex);
		if (!playAgain()) exit(0);		
	}
	return 0;
}