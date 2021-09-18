/*	Main.cpp
	This file contains the main function.
	
	Author: Tristan Pior
	Class: CS4375.002
*/

#include <iostream>
#include <fstream>
#include "DecisionTree.cpp"

using namespace std;

int main(int argc, char** argv) {
	//If there is an improper number of arguments, then terminate.
	if (argc != 3) {
		cout << "Invalid number of arguments. Requires 1 training file and 1 testing file.";
		return 1;
	}

	//Variables to store the file names
	string trainFile = argv[1];
	string testFile = argv[2];

	//Create a decision tree
	DecisionTree dt;
	dt.input(trainFile);
	vector<int> temp = {};
	dt.buildTree();
	dt.printTree();
	printf("\nAccuracy on training set (%i instances): %.1f%%\n", numTotalItems, dt.getAccuracy());
	
	
	
	
	
	//Wait at the end so the user can view the console window
	cout << "Press any key to continue . . .";
	cin.get();
}