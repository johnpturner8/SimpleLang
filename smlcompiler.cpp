// smlcompiler.cpp
// Authors: David Croft and John Turner
// Purpose: Compiles simple code inputs from a given file to convert to simple machine language and inputs it into another file
#include <iostream>
#include <array>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <vector>
using namespace std;

// *CONSTANTS*
const int symbolTableSize(100); //Sets the size of the symbol table (most likely that it won't reach this size)
const int smlBufferSize(100); //Sets the size of the SML buffer array

// Declares the symbol, type, and location columns of the symbol table
array<string,symbolTableSize> symbolColumn;
array<string,symbolTableSize> typeColumn;
array<int,symbolTableSize> locColumn;

// Declares the array that acts as the buffer that contains the translated SML code
array<int,smlBufferSize> smlBuffer;

//Declares data, symbol table, and buffer counters
int symbolCount = 0;
int bufferCount = 0;
int dataCount = 99;

bool isNumber(string s) 
{ 
    for (int i = 0; i < s.length(); i++) 
        if (isdigit(s[i]) == false) 
            return false;
    return true; 
}

//Function that returns the location in memory of the specified variable, and if it does not exist but create is "true", then it will create that variable. Used with "input" command
int var(string vname, bool create){
	for(int i = 0; i < symbolTableSize; i++)
	{
		if(symbolColumn[i] == vname && typeColumn[i] == "V")
		{
			return locColumn[i];
		}
	}
	if(!create)
	{
//		cerr<<"Variable " + vname + " does not exist.";
		return -1;
	}
	symbolColumn[symbolCount] = vname;
	typeColumn[symbolCount] = "V";
	locColumn[symbolCount] = dataCount;
	symbolCount++;
	dataCount--;
	return dataCount+1;
}

//Function that iterates through the symbol table and finds if the given symbol exists and returns its location in the smlbuffer if it does and returns a negative number if not. Used with "goto" command
int findSymbol(string s, int command)
{
	stringstream ss(s);
	int sym = 0;
	ss >> sym;
	for(int i = 0; i < symbolTableSize; i++)
	{
		if(symbolColumn[i] == s)
		{
			return command + locColumn[i];
		}
	}
	return -(sym + command);
}

vector<string> toPostFix(string exp){
	stringstream ss(exp);
	vector<string> theSplit;
	vector<string> theEnd;
	string s;
	while(!ss.eof()){
		ss >> s;
		theSplit.push_back(s);
		
	}
	int position = 1;
	theEnd.push_back(theSplit[0]);
	for(int i = 1; i<theSplit.size()-1; i++){
		if(theSplit[i] == "+"){
			theEnd.insert(theEnd.begin() + position, theSplit[i+1]);
			position++;
			theEnd.insert(theEnd.begin() + position, "+");
			i++;
		}
		else if(theSplit[i] == "-"){
			theEnd.insert(theEnd.begin() + position, theSplit[i+1]);
			position++;
			theEnd.insert(theEnd.begin() + position, "-");
			i++;
		}
		else if(theSplit[i] == "*"){
			theEnd.insert(theEnd.begin() + position, theSplit[i+1]);
			position++;
			theEnd.insert(theEnd.begin() + position, "*");
			position++;
			i++;
		}
		else if(theSplit[i] == "/"){
			theEnd.insert(theEnd.begin() + position, theSplit[i+1]);
			position++;
			theEnd.insert(theEnd.begin() + position, "/");
			position++;
			i++;
		}
	}
	return theEnd;
}

int constEval(string name){
	stringstream ss(name);
	if(isNumber(name)){
		int num;
		ss >> num;
		cout<<num<<endl;
		if(num > 99 || num < 0){
			cerr << "Constant " << num << " cannot be compiled." << endl;
			exit(EXIT_FAILURE);
		}
		smlBuffer[bufferCount] = 2200+num;
		cout<<smlBuffer[bufferCount]<<endl;
		bufferCount++;
		int loc = var(name, true);
		smlBuffer[bufferCount] = 2100+loc;
		cout<<smlBuffer[bufferCount]<<endl;
		bufferCount++;
		return loc;
	}
	else
		return var(name, false);
}


int evalExpression(vector<string> exp){ // Takes in a POSTFIX expression and evaluates it
	string validOps = "+-*/";
	int position = -1;
	if(exp.size() == 1){
		int ret = constEval(exp[0]);
		cout<<exp[0]<<endl;
		if(ret == -1){
			cerr<<"Variable "<<exp[0]<<" does not exist."<<endl;
			exit(EXIT_FAILURE);
		}
		return ret;
	}
	else if((validOps.find(exp[exp.size() - 1]) != -1) && (exp[exp.size() - 1].length() == 1)){
		int opcount = 0;
		int ncount = 0;
		int brek = -1;
		for(int i = exp.size()-2; i >= 0; i--)
		{
			if((validOps.find(exp[i]) != -1) && (exp[i].length() == 1))
			{
				opcount++;
			}
			else{
				ncount++;
			}
			if(ncount == opcount + 1){
				brek = i;
				break;	
			}
		}
		vector<string> sub1;
		vector<string> sub2;
		for(int j = 0; j < brek; j++){
			sub1.push_back(exp[j]);
		}
		for(int n = brek; n < exp.size() - 1; n++){
			sub2.push_back(exp[n]);
		}
		int lop1 = evalExpression(sub1);
		int lop2 = evalExpression(sub2);
		if(exp[exp.size() - 1] == "+"){
			smlBuffer[bufferCount] = 2000 + lop1;
			bufferCount++;
			smlBuffer[bufferCount] = 3000 + lop2;
			bufferCount++;
			smlBuffer[bufferCount] = 2100 + dataCount;
			dataCount--;
			bufferCount++;
			return dataCount + 1; 
		}
		else if(exp[exp.size() - 1] == "-"){
			smlBuffer[bufferCount] = 2000 + lop1;
			bufferCount++;
			smlBuffer[bufferCount] = 3100 + lop2;
			bufferCount++;
			smlBuffer[bufferCount] = 2100 + dataCount;
			dataCount--;
			bufferCount++;
			return dataCount + 1; 
		}
		else if(exp[exp.size() - 1] == "/"){
			smlBuffer[bufferCount] = 2000 + lop1;
			bufferCount++;
			smlBuffer[bufferCount] = 3200 + lop2;
			bufferCount++;
			smlBuffer[bufferCount] = 2100 + dataCount;
			dataCount--;
			bufferCount++;
			return dataCount + 1; 
		}
		else if(exp[exp.size() - 1] == "*"){
			smlBuffer[bufferCount] = 2000 + lop1;
			bufferCount++;
			smlBuffer[bufferCount] = 3300 + lop2;
			bufferCount++;
			smlBuffer[bufferCount] = 2100 + dataCount;
			dataCount--;
			bufferCount++;
			return dataCount + 1; 
		}
		/*else{
			cerr<"Error 6793"<<endl;
			return -6793;
		}*/
	}
	else{
		cerr<<"Error in expression";
		exit(EXIT_FAILURE);
	}
	//for(int i = 0; i < exp.size(); i++)
	
	
	//TODO Make the loop where the operators and operands will be assorted from infix to post fix
	return 0;
}

int main()
{
	// *CONSTANTS*
	const string IN_FILE_NAME = "smlcompilerinput.txt"; // Sets the file name for the compiler to take in
	const string OUT_FILE_NAME = "out.txt";

	// *MAIN CODE*
	//Opens the file to be read as "codeFile", and if unable to open, exits the program with error message
	ifstream codeFile(IN_FILE_NAME, ios::in);
	codeFile.clear();
	if(!codeFile.is_open())
	{
		cerr<<"Unable to open file."<<endl;
		exit(EXIT_FAILURE);
	}

	//Takes in first line of code from file and then starts iterating through file and adding each line to a place in memory, and once finished, closes the file
	string p1; // First part of string
	string p2; // Second part of string
	string p3; // Third part of string
	string line;
	//codeFile>>line;
	
	int index = 0;
	getline(codeFile, line);
	while(!codeFile.eof())
	{
		stringstream ss(line);
		ss >> p1;
		ss >> p2;
		getline(ss, p3);
		if(p3.size() >= 1){
			p3 = p3.substr (1);
		}
		if(!isNumber(p1)){
			cerr<<p1<<" is not a line number."<<endl;
			exit(EXIT_FAILURE);
		}
		else{
			stringstream ss2(p1);
			int checkNum;
			ss2 >> checkNum;
			if(checkNum < 0 || checkNum > 99){
				cerr<<p1<<" is an invalid line number."<<endl;
				exit(EXIT_FAILURE);
			}
		}
		if(p2 == "input")
		{
			symbolColumn[symbolCount] = p1;
			typeColumn[symbolCount] = "L";
			locColumn[symbolCount] = bufferCount;
			symbolCount++;
			smlBuffer[bufferCount] = 1000+var(p3, true);
			bufferCount++;
		}
		else if(p2 == "print")
		{
			symbolColumn[symbolCount] = p1;
			typeColumn[symbolCount] = "L";
			locColumn[symbolCount] = bufferCount;
			symbolCount++;
			int loc = evalExpression(toPostFix(p3));
			smlBuffer[bufferCount] = 1100+loc;
			//smlBuffer[bufferCount] = 1100+var(p3, false);
			bufferCount++;	
		}
		else if(p2 == "end")
		{
			symbolColumn[symbolCount] = p1;
			typeColumn[symbolCount] = "L";
			locColumn[symbolCount] = bufferCount;
			symbolCount++;
			smlBuffer[bufferCount] = 4300;
			bufferCount++;
		}
		else if(p2 == "rem")
		{
			symbolColumn[symbolCount] = p1;
			typeColumn[symbolCount] = "L";
			locColumn[symbolCount] = bufferCount;
		}
		else if(p2 == "goto")
		{
			symbolColumn[symbolCount] = p1;
			typeColumn[symbolCount] = "G";
			locColumn[symbolCount] = bufferCount;
			symbolCount++;
			smlBuffer[bufferCount] = findSymbol(p3, 4000);
			bufferCount++;
		}
		else if(p2 == "if")
		{
			//Make a way to be able to tell if the variable is a number or a string
			stringstream ss(p3);
			string tk1;
			string comp;
			string tk2;
			string s;
			string line;
			bool compFound = false;
			bool gotoFound = false;
			bool lineNumFnd = false;
			while(!ss.eof()){
				ss >> s;
				if(s == ">" || s == "<" || s == "="){
					comp = s;
					compFound = true;
				}
				else if(gotoFound){
					line = s;
					lineNumFnd = true;
				}
				else if(compFound){
					if(s == "goto"){
						gotoFound = true;
					}
					else{
						tk2 += s;
					}
				}
				else{
					tk1 += s;
				}		
			}
			if(!compFound){
				cerr<<"No comparator found in if statement on line "<<p1<<endl;
				exit(EXIT_FAILURE);
			}
			if(!gotoFound){
				cerr<<"No goto after if statement on line "<<p1<<endl;
				exit(EXIT_FAILURE);
			}
			if(!lineNumFnd){
				cerr<<"No line to goto on line "<<p1<<endl;
				exit(EXIT_FAILURE);
			}
			locColumn[symbolCount] = bufferCount;
			symbolColumn[symbolCount] = p1;
			typeColumn[symbolCount] = "G";
			symbolCount++;
			if(comp == ">")
			{
				int ltk1 = evalExpression(toPostFix(tk1));
				int ltk2 = evalExpression(toPostFix(tk2));
				smlBuffer[bufferCount] = 2000 + ltk2;
				bufferCount++;
				smlBuffer[bufferCount] = 3100 + ltk1;
				bufferCount++;
				smlBuffer[bufferCount] = findSymbol(line, 4100);
				bufferCount++;
			}
			else if(comp == "<")
			{
				int ltk1 = evalExpression(toPostFix(tk1));
				int ltk2 = evalExpression(toPostFix(tk2));
				smlBuffer[bufferCount] = 2000 + ltk1;
				bufferCount++;
				smlBuffer[bufferCount] = 3100 + ltk2;
				bufferCount++;
				smlBuffer[bufferCount] = findSymbol(line, 4100);
				bufferCount++;
			}
			else if(comp == "=")
			{
				int ltk1 = evalExpression(toPostFix(tk1));
				int ltk2 = evalExpression(toPostFix(tk2));
				smlBuffer[bufferCount] = 2000 + ltk1;
				bufferCount++;
				smlBuffer[bufferCount] = 3100 + ltk2;
				bufferCount++;
				smlBuffer[bufferCount] = findSymbol(line, 4200);
				bufferCount++;
			}
		}
		else if(p2 == "let")
		{
			stringstream ss(p3);
			string varble;
			string exp;
			ss >> varble >> exp;
			getline(ss, exp);
			exp = exp.substr (1);
			int pline = var(varble, false);
			int buf = evalExpression(toPostFix(exp));
			if(pline == -1){
				symbolColumn[symbolCount] = varble;
				typeColumn[symbolCount] = "V";
				locColumn[symbolCount] = dataCount;
				symbolCount++;
				smlBuffer[bufferCount] = 2000 + buf;
				bufferCount++;
				smlBuffer[bufferCount] = 2100 + dataCount;
				bufferCount++;
				dataCount--;
			}
			else{
				for(int i = 0; i < symbolTableSize; i++)
				{
					if(symbolColumn[i] == varble && typeColumn[i] == "V")
					{
						locColumn[i] = buf;
						break;
					}
				}
			}
		}
		else{
			cerr << "Invalid command at line " << p1 << endl;
			exit(EXIT_FAILURE);
		}
		index++;
		getline(codeFile, line);
	}

	for(int i = 0; i < symbolTableSize; i++)
	{
		if(typeColumn[i] == "G")
		{
			if(smlBuffer[locColumn[i]] < 0){
				string sym = to_string((-1*smlBuffer[locColumn[i]])%100);
				smlBuffer[locColumn[i]] = findSymbol(sym, ((-1*smlBuffer[locColumn[i]])/100)*100);
				if(smlBuffer[locColumn[i]] < 0){
					cerr << "You put a goto to an line that isn't there." << endl;
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	codeFile.close();
	if(dataCount <= bufferCount){
		cerr<<"Memory overflow."<<endl;
		exit(EXIT_FAILURE);
	}
	ofstream outFile(OUT_FILE_NAME,ios::in | ios::trunc);
	if(!outFile.is_open())
	{
		cerr<<"Unable to open file."<<endl;
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < smlBufferSize; i++){
		if(smlBuffer[i] == 0)
			break;
		outFile << smlBuffer[i] << "\n";
	}
	outFile.close();
}






