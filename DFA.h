#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

using namespace std;

class DFA
{
private:
	void tokenize(string str, vector<string> &tokens) {
		istringstream ss(str);
		while (!ss.eof()) {
		  string x;
		  getline( ss, x, ' ' );
		  tokens.push_back(x);
		}
	}
public:
	DFA(string _start, string _fin, string _alphabet, string _transTable) {

		string line;
		string alpha;
		string from;
		string to;
		int index;

		start = _start;
		tokenize(_fin, finalStates);
		tokenize(_alphabet, alphabet);
		transitions.resize(alphabet.size());

		stringstream ss;
		ss << _transTable;

		while (getline(ss, line)) {
			transitionTokens.clear();
			tokenize(line, transitionTokens);
			from = transitionTokens[0];
			alpha = transitionTokens[1];
			to = transitionTokens[2];

			// find which index contains "alpha" in the vector "alphabet"
			for (unsigned int i = 0; i < alphabet.size(); ++i) {
				if (alphabet[i] == alpha) {
					index = i;
					break;
				}
			}

			// then map the string "from" to "to", in the map at index "index" in vector "transitions".
			transitions[index].insert(pair<string,string>(from, to));
		}
	}
	virtual ~DFA() { }
	
	bool inLanguage(string word) const  // returns true if the word passed in is in the language defined by the DFA
	{
		string currentState = start;
		int index = 0;

		for (unsigned int i = 0; i < word.length(); ++i) {
			auto it = find(alphabet.begin(), alphabet.end(), string(1, word[i]));
			if (it != alphabet.end()) {
				index = it - alphabet.begin();
				if (transitions[index].find(currentState) != transitions[index].end()) {
					currentState = transitions[index].at(currentState);
				} else {
					return false;
				}
			}
			else {
				return false;
			}
		}
		
		// if current state is a final state, word was valid
		auto it2 = find(finalStates.begin(), finalStates.end(), currentState);
		return  (it2 != finalStates.end());
	}

private:
	vector<string> finalStates;
	vector<string> alphabet;
	vector<map<string, string>> transitions;
	vector<string> transitionTokens;
	ifstream file;
	string start;	
};