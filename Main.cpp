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
	//Input the training data
	dt.input(trainFile);
	//Set the most frequent class value
	dt.setMostFrequentClassValue();
	//Build the tree with the training data
	dt.buildTree();
	//Print the built tree
	dt.printTree();
	//Print the accuracy on the training data set
	printf("\nAccuracy on training set (%i instances): %.2f%%\n", numTotalItems, dt.getAccuracy());
	//Empty the item lists in all tree nodes
	dt.clearTree();
	//Input the test data
	dt.input(testFile);
	//Classify the test data
	dt.classifyTestData();
	//Print the accuracy on the test data set
	printf("\nAccuracy on test set (%i instances): %.2f%%\n", numTotalItems, dt.getAccuracy());

	
	
	
	
	//Wait at the end so the user can view the console window
	cout << "Press any key to continue . . .";
	cin.get();
}