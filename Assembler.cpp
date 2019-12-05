#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <array>
using namespace std;

int main(){
	const int MEMSIZE = 100;
	array<int, MEMSIZE> memory = {};
	string filename;
	cout << "Enter the name of the file to be processed: ";
	cin >> filename;
	ifstream inFile (filename, ios::in);
	if(!inFile) {
		cerr << "File not able to be opened" << endl;
		exit(EXIT_FAILURE);
	}
	int command;
	inFile >> command;
	int index = 0;
	while(!inFile.eof()) {
		memory[index] = command;
		index++;
		inFile >> command;
	}
	
	const int READ = 10;
	const int WRITE = 11;
	const int LOAD = 20;
	const int STORE = 21;
	const int ACCUMULATE = 22;
	const int ADD = 30;
	const int SUBTRACT = 31;
	const int DIVIDE = 32;
	const int MULTIPLY = 33;
	const int BRANCH = 40;
	const int NEGBRANCH = 41;
	const int ZEROBRANCH = 42;
	const int HALT = 43;
	
	int accumulator = 0;
	for(int i = 0; i < MEMSIZE; i++){
		switch(memory[i]/100){
			case READ:
				cin>>memory[memory[i]%100];
				break;
			case WRITE:
				cout<<memory[memory[i]%100]<<endl;
				break;
			case LOAD:
				accumulator = memory[memory[i]%100];
				break;
			case STORE:
				memory[memory[i]%100] = accumulator;
				break;
			case ACCUMULATE:
				{
				accumulator = memory[i]%100;
				}
				break;
			case ADD:
				accumulator += memory[memory[i]%100];
				break;
			case SUBTRACT:
				accumulator -= memory[memory[i]%100];
				break;
			case MULTIPLY:
				accumulator *= memory[memory[i]%100];
				break;
			case DIVIDE:
				accumulator /= memory[memory[i]%100];
				break;
			case BRANCH:
				i = memory[i]%100 - 1;
				break;
			case NEGBRANCH:
				{
					if(accumulator < 0){
						i = memory[i]%100 - 1;
					}
				}
				break;
			case ZEROBRANCH:
				{
					if(0 == accumulator){
						i = memory[i]%100 - 1;
					}
				}
				break;
			case HALT:
				return 0;
		}
	}
}
