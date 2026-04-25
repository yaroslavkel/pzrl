#include "BST.h"
BinarySearchTree::Node::Node(Key key, Value value, Node* parent, Node* left, Node* right) : 
keyValuePair(key, value), parent(parent), left(left), right(right) {}
BinarySearchTree::Node::Node(const Node& other) : 
keyValuePair(other.keyValuePair), parent(nullptr), left(nullptr), right(nullptr) {}
bool BinarySearchTree::Node::operator==(const Node& other) const {
	return keyValuePair == other.keyValuePair;
}
void BinarySearchTree::Node::insert(const Key &key, const Value &value){
	if (key <= keyValuePair.first){
		if (left){
			left->insert(key, value);
		}
		else {
			left = new Node(key, value, this);
		}
	}
	else {
		if (right){
			right->insert(key, value);
		}
		else {
			right = new Node(key, value, this);
		}
	}
}
void BinarySearchTree::Node::erase(const Key& key){
	Node* toDelete = nullptr;
	if (key < keyValuePair.first){
		if (left){
			left->erase(key);
			return;
		}
	}
	else if (key > keyValuePair.first){
		if (right){
			right->erase(key);
			return;
		}
	}
	toDelete = this;
	if (toDelete->left && toDelete->right){
		Node* minR = toDelete->right;
		while (minR->left){
			minR = minR->left;
		}
		toDelete->keyValuePair = minR->keyValuePair;
		if (minR->parent->left == minR){
			minR->parent->left = minR->right;
		}
		else {
			minR->parent->right = minR->right;
		}
		if (minR->right){
			minR->right->parent = minR->parent;
		}
		delete minR;
		return;
	}
	Node* child = toDelete->left ? toDelete->left : toDelete->right;
	if (toDelete->parent){
		if (toDelete->parent->left == toDelete){
			toDelete->parent->left == child;
		}
		else {
			toDelete->parent->right = child;
		}
	}
	if (child){
		child->parent = toDelete->parent;
	}
	delete toDelete;
}
void BinarySearchTree::Node::output_node_tree() const {}
BinarySearchTree::BinarySearchTree() {}
BinarySearchTree::BinarySearchTree(const BinarySearchTree& other) : 

