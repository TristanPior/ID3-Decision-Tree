/*	ItemTree.cpp
	This file contains the implementation of the Item class and the DTreeNode class.
	Item will hold a list of the values for each attribute for a specific item in the input list.
	DTreeNode will be a node that stores what attribute that it contains, a list of Items that are contained in the node, and a list of child nodes.

	Author: Tristan Pior
	Class: CS4375.002
*/

#include <vector>

using namespace std;

class Item;

static vector<Item> itemList = {};

//Item
class Item {
		 //Vector to store a variable number of values of attributes.
private: vector<int> attVal = {};
		 //Variable to store the class value. This will be set by the constructor if a class value is known from the input, or assigned by classification.
private: int classVal = -1;

		//Known class value constructor
public: Item(vector<int> values, int cVal) {
		attVal = values;
		classVal = cVal;
	}

		//Unknown class value constructor
public: Item(vector<int> values) { attVal = values; }

		//Get function for attVal
public: vector<int> getAttVal() { return attVal; }

		//Get function for classVal
public: int getClassVal() { return classVal; }

};



//DTreeNode
class DTreeNode {
		 //Attribute being split
private: string attribute = "";
		 //Attribute's value
private: int attVal = -1;
		 //Vector of Items in this node
private: vector<Item> iList = {};
		 //Vector of child DTreeNodes
private: vector<DTreeNode*> children = {};
		 //Stores the expected class value at this leaf node
		 //ONLY USED IN LEAF NODES
private: int expectedClassValue = -1;

		//Source Node Constructor
public: DTreeNode() {
	attribute = "";
	attVal = -1;
	iList = {};
	children = {};
};
		//Child Constructor
public: DTreeNode(string att, int aVal) {
		attribute = att;
		attVal = aVal;
		iList = {};
		children = {};
	}

		//Function to add an item to the node
public: void addItem(Item i) { this->iList.push_back(i); }

		//Function to add a child to this node
public: void addChild(string attribute, int aVal) {
		DTreeNode* child = new DTreeNode(attribute, aVal);
		this->children.push_back(child);
	}

		//Get function for list of items
public: vector<Item> getItems() { return iList; }

		//Get function for children
public: vector<DTreeNode*> getChildren() { return children; }

		//Get function for expected class value
public: int getExpectedClassValue() { return expectedClassValue; }

		//Set function for expected class value
public: void setExpectedClassValue(int n) { this->expectedClassValue = n; }

		//Get function for attribute string
public: string getAttribute() { return attribute; }

		//Get function for attribute value
public: int getAttributeValue() { return attVal; }

		//Function to clear the items list (used by the Testing phase to remove the training data from the tree)
		// *ONLY CALL FROM THE SRC NODE (RECURSIVE FUNC)*
public : void clearIList() { 
		iList = {};
		for (size_t i = 0; i < children.size(); i++) { children[i]->clearIList(); }
	}
};