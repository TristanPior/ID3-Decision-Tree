/*	DecisionTree.cpp
	This file contains the implementation of the ID3 algorithm to train on a training dataset.

	Author: Tristan Pior
	Class: CS4375.002
*/

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include "ItemTree.cpp"

//Decision Tree source node
//static DTreeNode srcNode;
static vector<string> attributes = {};
static int mostFrequentClassVal;
static int numTotalItems = 0;

class DecisionTree {
private: DTreeNode n;

		//DecisionTree constructor
public: DecisionTree() {

}

		 //Function to input data from file
public: void input(string fileName) {
	//Vector of items in the input file
	//vector<Item> itemList = {};
	//Input stream to read from file
	ifstream read(fileName);
	string line;
	string token;
	//Counter to know the number of attrbutes before class
	int count = 0;

	//Counters for the three possible values for class value
	int c0 = 0;
	int c1 = 0;
	int c2 = 0;

	//Populate line for use in instantiatingthe string stream
	getline(read, line);
	//String stream for parsing lines from the file
	stringstream ss;
	ss << line;
	//Populate line with the line of attributes

	//Populate attributes vector and count them
	while (!ss.eof()) {
		ss >> token;
		if (token != "class") {
			attributes.push_back(token);
			token = "";
			count++;
		}
		
	}

	//Populate itemList
	while (getline(read, line)) {
		vector<int> attV;
		ss.str("");
		ss.clear();
		ss << line;
		attV = {};
		int temp = 0;
		int i = 0;
		while (i < count) {
			ss >> temp;
			attV.push_back(temp);
			i++;
		}
		int classV = -1;
		ss >> classV;
		//Increment the counters for determining most common class value
		switch (classV) {
			case 0: c0++;
				break;
			case 1: c1++;
				break;
			case 2: c2++;
				break;
		}
		Item it(attV, classV);
		itemList.push_back(it);
		Item* iPtr = &itemList.back();
		numTotalItems++;
		this->n.addItem(it);
	}
	//Determine most frequent class value
	if (c0 >= c1 && c0 >= c2) { mostFrequentClassVal = c0; }
	else if (c1 > c0 && c1 >= c2) { mostFrequentClassVal = c1; }
	else/* if (c2 > c0 && c2 > c1)*/ { mostFrequentClassVal = c2; }
}

		 //Helper function for getInfoGain to calculate entropy
private: double getEntropy(vector<Item> iPointers) {
	//Check for empty vector
	if (iPointers.size() == 0) { return 0; }
	//Total number of items in iPointers
	int totalItems = 0;
	//Total number of items that belong with class value 0, 1, or 2
	double v0 = 0;
	double v1 = 0;
	double v2 = 0;
	//Loop through iPointers
	for (size_t i = 0; i < iPointers.size(); i++) {
		Item temp = iPointers[i];
		totalItems++;
		int clVal = temp.getClassVal();
		//Switch for 0, 1, or 2 class values
		switch (clVal) {
		case 0: v0 += 1;
			break;
		case 1: v1 += 1;
			break;
		case 2: v2 += 1;
			break;
		}
	}
	//Calculate entropy
	double p0 = v0 / totalItems;
	double p1 = v1 / totalItems;
	double p2 = v2 / totalItems;

	double e0;
	double e1;
	double e2;

	//Check for 0 log2 0
	if (p0 == 0) { e0 = 0; }
	else { e0 = -1 * (p0 * log2(p0)); }
	//Check for 0 log2 0
	if (p1 == 0) { e1 = 0; }
	else { e1 = -1 * (p1 * log2(p1)); }
	//Check for 0 log2 0
	if (p2 == 0) { e2 = 0; }
	else { e2 = -1 * (p2 * log2(p2)); }

	return e0 + e1 + e2;
}

		 //Helper function to calculate information gain
private: double getInfoGain(vector<Item> iPointers, int attNum) {
	//Entropy variables
	double parentEntropy = getEntropy(iPointers);
	double e0;
	double e1;
	double e2;

	//Item pointer lists for attribute values
	vector<Item> iAtt0 = {};
	vector<Item> iAtt1 = {};
	vector<Item> iAtt2 = {};

	//Split items by attribute value
	for (size_t i = 0; i < iPointers.size(); i++) {
		Item temp = iPointers[i];
		int val = temp.getAttVal()[attNum];
		switch (val) {
			case 0: iAtt0.push_back(iPointers[i]);
				break;
			case 1: iAtt1.push_back(iPointers[i]);
				break;
			case 2: iAtt2.push_back(iPointers[i]);
				break;
		}
	}
	//Calculate entropy of each value
	e0 = getEntropy(iAtt0);
	e1 = getEntropy(iAtt1);
	e2 = getEntropy(iAtt2);

	//Compute probability of each value
	double p0 = (double) iAtt0.size() / iPointers.size();
	double p1 = (double) iAtt1.size() / iPointers.size();
	double p2 = (double) iAtt2.size() / iPointers.size();

	//Return Information Gain
	return parentEntropy - ((e0 * p0) + (e1 * p1) + (e2 * p2));
}

		 //Helper function to set leaf node expected class value
private: void assignClassValue(DTreeNode* node) {
	vector<Item> iList = node->getItems();
	//If no items are in the node, then the class value is the most frequent class value
	if (iList.size() == 0) { node->setExpectedClassValue(mostFrequentClassVal); }
	//Counters for the 3 possible class values
	int c0 = 0;
	int c1 = 0;
	int c2 = 0;
	//Loop through the list of items and count the number of each class value
	for (Item i : iList) {
		int temp = i.getClassVal();
		switch (temp) {
			case 0: c0++;
				break;
			case 1: c1++;
				break;
			case 2: c2++;
				break;
		}
	}
	if (c0 >= c1 && c0 >= c2) { node->setExpectedClassValue(0); }
	else if (c1 > c0 && c1 >= c2) { node->setExpectedClassValue(1); }
	else { node->setExpectedClassValue(2); }
}

		 //Entry function to build the decision tree
public: void buildTree() {
	vector<int> temp = {};
	buildTreeHelper(&n, temp);
}

		 //Primary recursive function to build the decision tree off of srcNode
public: void buildTreeHelper(DTreeNode* node, vector<int> usedAttributes) {
	//Check for early leaf node condition
	vector<Item> iList = node->getItems();
	if ((iList.size() == 0) || (usedAttributes.size() == attributes.size())) {
		assignClassValue(node);
		return;
	}
	Item t = iList[0];
	int temp = t.getClassVal();
	//Boolean for determining if all items have the same class value
	bool same = true;
	for (size_t i = 0; (i < iList.size()) && same; i++) {
		Item ti = iList[i];
		if (ti.getClassVal() != temp) {
			same = false;
		}
	}
	//If all items have the same class value, then assign a class value to the leaf node and return
	if (same) {
		assignClassValue(node);
		return;
	}


	//Store the best information gain and the attribute with it
	double bestIGain = -1;
	int bestIGainAttribute = -1;
	//Calculate information gain for all unused attributes
	for (size_t i = 0; i < attributes.size(); i++) {
		bool used = false;
		for (size_t j = 0; j < usedAttributes.size(); j++) { if (usedAttributes[j] == i) { used = true; } }
		if (used) {}
		else {
			double temp = getInfoGain(node->getItems(), i);
			if (temp > bestIGain) {
				bestIGain = temp;
				bestIGainAttribute = i;
			}
		}
	}
	//Check if bestIGainAttribute is -1
	if (bestIGainAttribute == -1)
		return;
	//Set the best information gain attribute as used
	usedAttributes.push_back(bestIGainAttribute);
	//Create new children
	string attribute = attributes[bestIGainAttribute];
	node->addChild(attribute, 0);
	node->addChild(attribute, 1);
	node->addChild(attribute, 2);

	vector<DTreeNode*> children = node->getChildren();

	//Give children their respective items
	for (Item i : iList) {
		int val = i.getAttVal()[bestIGainAttribute];
		children[val]->addItem(i);
	}

	//Recursively call for all children
	for (size_t i = 0; i < children.size(); i++) { buildTreeHelper(children[i], usedAttributes); }
}

		 //Recursive helper function for printTree
private: string printTreeHelper(DTreeNode node, int depth) {
	//Output variable
	string output = "";
	//Format the depth
	for (int i = 0; i < depth; i++) { output += "| "; }
	output += node.getAttribute() += " = ";
	output += to_string(node.getAttributeValue()) += " :";
	//If the node is a leaf node then output the class value on the same line, else new line
	if (node.getExpectedClassValue() != -1) { output += " " + to_string(node.getExpectedClassValue()); }
	else { depth++; }
	//Get a vector of all child nodes
	vector<DTreeNode*> children = node.getChildren();
	
	output += "\n";

	//Loop through all children and recursively call printTreeHelper
	for (DTreeNode* c : children) { output += printTreeHelper(*c, depth); }
	return output;
}

		 //Output function to print the tree into the console
public: void printTree() {
	vector<DTreeNode*> children = n.getChildren();
	for (DTreeNode* c : children) { cout << printTreeHelper(*c, 0); }
}

		//Recursive Helper function for getAccuracy that returns the number of correctly classified items
private: int getAccuracyHelper(DTreeNode* node) {
	//Count to keep track of the number of correctly classified items
	int count = 0;
	if (node->getChildren().size() == 0) {
		int classVal = node->getExpectedClassValue();
		for (Item i : node->getItems()) { if (i.getClassVal() == classVal) { count++; }}
		return count;
	}
	for (DTreeNode* i : node->getChildren()) { count += getAccuracyHelper(i); }
	return count;
}
		
		//Function that outputs the accuracy of the classification as a percentage
public: double getAccuracy() {
	return ((double) getAccuracyHelper(&n) / numTotalItems) * 100.0;
}

};