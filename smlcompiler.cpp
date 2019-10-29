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
array<string,symbolTableSize> locColumn;

// Declares the array that acts as the buffer that contains the translated SML code
array<int,smlBufferSize> smlBuffer;

//Declares data, symbol table, and buffer counters
symbolCOunt = 0;
bufferCount = 0;
dataCount = 99;

int var(string vname){
	for(int i = 0; i < symbolTableSize; i++){
		
	}
}

int main()
{
	// *CONSTANTS*
	const string FILE_NAME = "smlcompileroutput.txt"; // Sets the file name for the compiler to take in

	// *MAIN CODE*
	//Opens the file to be read as "codeFile", and if unable to open, exits the program with error message
	ifstream codeFile(FILE_NAME,ios::in);
	if(!codeFile.is_open())
	{
		cerr<<"Unable to open file."<<endl;
		exit(EXIT_FAILURE);
	}

	//Takes in first line of code from file and then starts iterating through file and adding each line to a place in memory, and once finished, closes the file
	string p1; // First part of string
	string p2; // Second part of string
	string p3; // Third part of string
	codeFile>>p1;
	codeFile>>p2;
	codeFile>>p3;
	int index = 0;
	while(!codeFile.eof())
	{
		switch(p2){
			case input:
				{
				symbolColumn[symbolCount] = p1;
				typeColumn[symbolCount] = "L";
				locColumn[symbolCount] = bufferCount;
				smlBuffer[bufferCount] = 1000+var(p3);
				symbolCount++;
				symbolColumn[symbolCount] = p3;
				typeColumn[symbolCount] = "V"
				locColumn[symbolCount] = dataCount;
				bufferCount++;
				dataCount--;
				}
				break;
		}
		cout<<command<<endl;
		index++;
		codeFile>>command;
	}
	codeFile.close();

}
