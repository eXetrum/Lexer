#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "DFA.h"
#include "Scanner.h"

using namespace std;

void printUsage();
string readLines(string filepath);

int main(int argc, char *argv[])
{
	if(argc != 2) {
		printUsage();
		return 0;
	}	

	Scanner scanner;
	scanner.proceedText( readLines(argv[1]) );
	scanner.printResult();

	return 0;
}

void printUsage() { cout << "cxref.exe <input_file>" << endl; }

string readLines(string filepath) {
	ifstream file(filepath);
	if(!file.is_open()) { cout << "File " << filepath << " not found." << endl; }

	string text = "";
	string line = "";
	while (getline(file, line)) text += line + "\n";
	file.close();
	return text;
}