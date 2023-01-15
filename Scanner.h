#pragma once

class Scanner {
private:
	// returns true if the char argument is whitespace or a comma (since commas serve as delimiters in C)
	bool isWhitespace(char ch) { return (ch == ' ' || ch == '\t' || ch == '\n' || ch == ','); }
	// returns true if the char argument is a special symbol
	bool isSymbol(char ch) { return (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == ';' || ch == '[' || ch == ']'); }
	// returns true if the char argument is an operator
	bool isOperator(char ch) {
		static string operators = "+-=/*%<>";
		return operators.find(ch) != string::npos;
	}
	// returns true if token is a keyword
	bool isKeyword(string token) {
		static string keywords[] =
		{
			"auto", "break", "case", "char", "const", "continue",
			"default", "do", "double", "else", "enum", "extern",
			"float", "for", "goto", "if", "int", "long", "register", "return",
			"short", "signed", "sizeof", "static", "struct", "switch",
			"typedef", "union", "unsigned", "void", "volatile", "while"
		};
		int N = sizeof(keywords) / sizeof(string);
		return find(keywords, keywords + N, token) != (keywords + N);
	}
	// remove spaces from string
	string stripSpaces(string str) {
		string noSpaces;
		for (size_t i = 0; i < str.size(); i++) {
			if (str[i] != ' ') {
				noSpaces.push_back(str[i]);
			}
		}
		return noSpaces;
	}
	string join(vector<int> &values, string separator) {
		string result = "";
		for (size_t i = 0; i < values.size(); ++i) {
			result += to_string(values.at(i));
			if (i < values.size() - 1) result += separator;
		}
		return result;
	}
	void analyze(string token, int lineNumber) {
		if (token.length() == 0) return;

		if (isKeyword(token)) {
			lastIDENT = "";
			string validRetrun[] = 
			{ 
				"int", "long", "short", "void", "unsigned", "float", "double", "char", "signed"
			};
			int n = sizeof(validRetrun) / sizeof(string);
			bool found = false;
			for (int i = 0; i < n && !found; ++i) {
				if (token == validRetrun[i]) found = true;
			}
			
			if(found) keywordOccured = true;

			if (find(cxrefResult["Keyword"][token].begin(),
				cxrefResult["Keyword"][token].end(),
				lineNumber) == cxrefResult["Keyword"][token].end()) {
				cxrefResult["Keyword"][token].push_back(lineNumber);
			}

		}
		else if (dfaIdentifier->inLanguage(token)) {
			lastIDENT = token;
			if (keywordOccured) {
				definition_start = true;
				keywordOccured = false;
			}
			else
			{
				definition_start = false;
			}

			if (find(cxrefResult["Identifier"][token].begin(),
				cxrefResult["Identifier"][token].end(),
				lineNumber) == cxrefResult["Identifier"][token].end()) {
				cxrefResult["Identifier"][token].push_back(lineNumber);
			}

		}
		else if (dfaInteger->inLanguage(token)) {

			if (find(cxrefResult["Integer"][token].begin(),
				cxrefResult["Integer"][token].end(),
				lineNumber) == cxrefResult["Integer"][token].end()) {
				cxrefResult["Integer"][token].push_back(lineNumber);
			}

		}
		else if (dfaFloat->inLanguage(token)) {

			if (find(cxrefResult["Float"][token].begin(),
				cxrefResult["Float"][token].end(),
				lineNumber) == cxrefResult["Float"][token].end()) {
				cxrefResult["Float"][token].push_back(lineNumber);
			}

		}
		else if (dfaString->inLanguage(token)) {

			if (find(cxrefResult["String Literal"][token].begin(),
				cxrefResult["String Literal"][token].end(),
				lineNumber) == cxrefResult["String Literal"][token].end()) {
				cxrefResult["String Literal"][token].push_back(lineNumber);
			}

		}
		else {

			if (find(cxrefResult["Error"][token].begin(),
				cxrefResult["Error"][token].end(),
				lineNumber) == cxrefResult["Error"][token].end()) {
				cxrefResult["Error"][token].push_back(lineNumber);
			}
		}
	}

	string lastIDENT;
	bool keywordOccured;
	bool definition_start;

	map<string, int> functionsDef;
	map<string, vector<int>> functionsCall;
public:
	Scanner() {
		lastIDENT = "";
		keywordOccured = false;
		definition_start = false;

		map<string, string> transitionTable;
		transitionTable["STRING"] = "0 \" 1\n1 \" 0\n1 a 1\n1 b 1\n1 c 1\n1 d 1\n1 e 1\n1 f 1\n1 g 1\n1 h 1\n1 i 1\n1 j 1\n1 k 1\n1 l 1\n1 m 1\n1 n 1\n1 o 1\n1 p 1\n1 q 1\n1 r 1\n1 s 1\n1 t 1\n1 u 1\n1 v 1\n1 w 1\n1 x 1\n1 y 1\n1 z 1\n1 A 1\n1 B 1\n1 C 1\n1 D 1\n1 E 1\n1 F 1\n1 G 1\n1 H 1\n1 I 1\n1 J 1\n1 K 1\n1 L 1\n1 M 1\n1 N 1\n1 O 1\n1 P 1\n1 Q 1\n1 R 1\n1 S 1\n1 T 1\n1 U 1\n1 V 1\n1 W 1\n1 X 1\n1 Y 1\n1 Z 1\n1 ! 1\n1 @ 1\n1 # 1\n1 $ 1\n1 % 1\n1 ^ 1\n1 & 1\n1 * 1\n1 ( 1\n1 ) 1\n1 _ 1\n1 - 1\n1 + 1\n1 = 1\n1 { 1\n1 } 1\n1 [ 1\n1 ] 1\n1 \\ 1\n1 | 1\n1 ; 1\n1 : 1\n1 ' 1\n1 \" 1\n1 / 1\n1 ? 1\n1 , 1\n1 < 1\n1 . 1\n1 > 11 0 1\n1 1 1\n1 2 1\n1 3 1\n1 4 1\n1 5 1\n1 6 1\n1 7 1\n1 8 1\n1 9 1";
		transitionTable["IDENTIFIER"] = "0 a 1\n0 b 1\n0 c 1\n0 d 1\n0 e 1\n0 f 1\n0 g 1\n0 h 1\n0 i 1\n0 j 1\n0 k 1\n0 l 1\n0 m 1\n0 n 1\n0 o 1\n0 p 1\n0 q 1\n0 r 1\n0 s 1\n0 t 1\n0 u 1\n0 v 1\n0 w 1\n0 x 1\n0 y 1\n0 z 1\n0 A 1\n0 B 1\n0 C 1\n0 D 1\n0 E 1\n0 F 1\n0 G 1\n0 H 1\n0 I 1\n0 J 1\n0 K 1\n0 L 1\n0 M 1\n0 N 1\n0 O 1\n0 P 1\n0 Q 1\n0 R 1\n0 S 1\n0 T 1\n0 U 1\n0 V 1\n0 W 1\n0 X 1\n0 Y 1\n0 Z 1\n0 _ 1\n1 a 1\n1 b 1\n1 c 1\n1 d 1\n1 e 1\n1 f 1\n1 g 1\n1 h 1\n1 i 1\n1 j 1\n1 k 1\n1 l 1\n1 m 1\n1 n 1\n1 o 1\n1 p 1\n1 q 1\n1 r 1\n1 s 1\n1 t 1\n1 u 1\n1 v 1\n1 w 1\n1 x 1\n1 y 1\n1 z 1\n1 A 1\n1 B 1\n1 C 1\n1 D 1\n1 E 1\n1 F 1\n1 G 1\n1 H 1\n1 I 1\n1 J 1\n1 K 1\n1 L 1\n1 M 1\n1 N 1\n1 O 1\n1 P 1\n1 Q 1\n1 R 1\n1 S 1\n1 T 1\n1 U 1\n1 V 1\n1 W 1\n1 X 1\n1 Y 1\n1 Z 1\n1 _ 1\n1 0 1\n1 1 1\n1 2 1\n1 3 1\n1 4 1\n1 5 1\n1 6 1\n1 7 1\n1 8 1\n1 9 1";
		transitionTable["INTEGER"] = "0 1 0\n0 2 0\n0 3 0\n0 4 0\n0 5 0\n0 6 0\n0 7 0\n0 8 0\n0 9 0\n0 0 0";
		transitionTable["FLOAT"] = "0 0 0\n0 1 0\n0 2 0\n0 3 0\n0 4 0\n0 5 0\n0 6 0\n0 7 0\n0 8 0\n0 9 0\n0 . 1\n1 . 2\n1 0 1\n1 1 1\n1 2 1\n1 3 1\n1 4 1\n1 5 1\n1 6 1\n1 7 1\n1 8 1\n1 9 1";

		map<string, string> alphabet;
		alphabet["STRING"] = "a b c d e f g h i j k l m n o p q r s t u v w x y z A B C D E F G H I J K L M N O P Q R S T U V W X Y Z , < . > / ? ; : ' \" [ { ] } 1 2 3 4 5 6 7 8 9 0 - _ = + \\ | ! @ # $ % ^ & * ( )";
		alphabet["IDENTIFIER"] = "0 1 2 3 4 5 6 7 8 9 a b c d e f g h i j k l m n o p q r s t u v w x y z A B C D E F G H I J K L M N O P Q R S T U V W X Y Z _";
		alphabet["INTEGER"] = "0 1 2 3 4 5 6 7 8 9";
		alphabet["FLOAT"] = "0 1 2 3 4 5 6 7 8 9 .";

		dfaString = new DFA("0", "0", alphabet["STRING"], transitionTable["STRING"]);
		dfaIdentifier = new DFA("0", "1", alphabet["IDENTIFIER"], transitionTable["IDENTIFIER"]);
		dfaInteger = new DFA("0", "0", alphabet["INTEGER"], transitionTable["INTEGER"]);
		dfaFloat = new DFA("0", "1", alphabet["FLOAT"], transitionTable["FLOAT"]);
	}

	

	void proceedText(string inputText) {
		functionsDef.clear();
		functionsCall.clear();
		cxrefResult.clear();
		stringstream ss(inputText);
		string line;		// The line to be read in from file
		string token;		// The token to be analyzed
		char lookahead;		// Char that looks ahead for spaces and symobls
		int lineNum = 1;	// Current processed line num
		bool multiline_comment = false;
		bool func_start = false;

		while (getline(ss, line)) {

			for (size_t i = 0; i < line.size(); i++) {
				lookahead = line[i];
				// Multiline comment continue
				if (multiline_comment) {					
					while (i < line.size()) {
						//cout << lookahead;
						lookahead = line[i];
						if (lookahead == '*' && i + 1 < line.size() && line[i + 1] == '/')
						{
							multiline_comment = false;
							i++;
							//cout << endl;
							break;
						}
						i++;
					}
				}
				// Multiline comment start
				else if (lookahead == '/' && i < line.size() - 1 && line[i + 1] == '*') {
					i++;
					multiline_comment = true;
				}
				// Single line comment start
				else if (lookahead == '/' && i < line.size() - 1 && line[i + 1] == '/') {
					while (lookahead != '\n' && i < line.size()) {
						i++;
						lookahead = line[i];
					}
					token.clear();
				}
				else if (isWhitespace(lookahead)) {
					// token can be analyzed since we hit a whitespace
					analyze(token, lineNum);
					token.clear();
				}
				else if (isSymbol(lookahead)) {
					// token can be analyzed since we hit a symbol
					analyze(token, lineNum);
					token.clear();
					// Function definition ???
					if (lastIDENT != "" && definition_start && lookahead == '(') {
						functionsDef[lastIDENT] = lineNum;
					}
					else if (lastIDENT != "" && lookahead == '(') {// Function call ???
						//cout << "CALL: " << lastIDENT << " " << lineNum << endl;
						if (functionsDef.find(lastIDENT) != functionsDef.end()) {
							functionsCall[lastIDENT].push_back(lineNum);
						}
					}
					
					lastIDENT = "";
					keywordOccured = false;
					definition_start = false;
				}
				else if (isOperator(lookahead)) {
					// token can be analyzed since we hit an operator
					analyze(token, lineNum);
					token.clear();
				}
				else if (lookahead == '"') {
					// token can be analyzed since we hit the start of a string
					analyze(token, lineNum);
					token.clear();

					token.push_back(lookahead);
					i++;
					lookahead = line[i];

					while (lookahead != '"') {
						token.push_back(lookahead);
						i++;
						lookahead = line[i];
					}

					token.push_back(lookahead);
				}

				else {
					// didn't hit a delimiter, so append lookahead to token
					token.push_back(lookahead);
				}
			}
			lineNum++;
		}

	}

	void printResult() {
		map<string, vector<int>> cur = cxrefResult["Identifier"];
		map<string, vector<int>>::iterator it = cur.begin();
		for (; it != cur.end(); ++it) {
			// Is function ?
			map<string, int>::iterator itter = functionsDef.find(it->first);
			if (itter == functionsDef.end()) continue;
			// Print name
			cout << setw(12) << it->first << " " << "defined at " << itter->second 
				// Print references
				<< ", references: " << right << join(functionsCall[it->first], ", ") << endl;
		}
	}



	virtual ~Scanner() {
		delete dfaString;
		delete dfaIdentifier;
		delete dfaInteger;
		delete dfaFloat;
	}
private:
	DFA *dfaString;
	DFA *dfaIdentifier;
	DFA *dfaInteger;
	DFA *dfaFloat;
	map<string, map<string, vector<int> > > cxrefResult;
};