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

int var(string vname, bool create){
	for(int i = 0; i < symbolTableSize; i++)
	{
		if(symbolColumn[i] == vname)
		{
			return locColumn[i];
		} 
	}
	if(!create)
	{
		cerr<<"Variable " + vname + " does not exist.";
	}
	symbolColumn[symbolCount] = vname;
	typeColumn[symbolCount] = "V";
	locColumn[symbolCount] = dataCount;
	symbolCount++;
	dataCount--;
	return dataCount+1;
}

int main()
{
	// *CONSTANTS*
	const string IN_FILE_NAME = "smlcompilerinput.txt"; // Sets the file name for the compiler to take in
	const string OUT_FILE_NAME = "smlcompileroutput.txt";

	// *MAIN CODE*
	//Opens the file to be read as "codeFile", and if unable to open, exits the program with error message
	ifstream codeFile(IN_FILE_NAME,ios::in);
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
	while(!codeFile.eof())
	{
		getline(codeFile, line);
		stringstream ss(line);
		ss >> p1;
		ss >> p2;
		getline(ss, p3);
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
			smlBuffer[bufferCount] = 1100+var(p3, false);
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
		index++;
	}

	codeFile.close();
	
	ofstream outFile(OUT_FILE_NAME,ios::in);
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






