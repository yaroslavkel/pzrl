#include "flist.h"
#include <iostream>
#include <stdexcept>
flist::flist() : head(nullptr) {}
void flist::display() const{
    Node* ptr = head;
    while (ptr != nullptr){
	std::cout << ptr->num;
	if (ptr->next != nullptr){
	    std::cout << " -> ";
	}
	ptr = ptr->next;
    }
    std::cout << std::endl;
}
flist::~flist(){
	if (!head) {
		std::cout << "The list is empty!" << std::endl;
		return;
	}
	while (head != nullptr){
		pop_back();
	}
}	
void flist::push_back(double n){
	Node* ptr = head;
	while (ptr && ptr->next){
		ptr = ptr->next;
	}
	Node* new_node = new Node{nullptr, n};
	if (ptr){
		ptr->next = new_node;
	}
	else {
		head = new_node;
	}
}
void flist::pop_back(){
	size_t len = size();
	if (len == 1){
		Node* sec_ptr = head->next;
		delete head;
		head = sec_ptr;
	}
	else if (len > 1){
		size_t k = 0;
		Node* ptr = head;
		while (k < len - 2){
			ptr = ptr->next;
			++k;
		}
		delete ptr->next;
		ptr->next = nullptr;
	}
}
size_t flist::size() const {
	Node* ptr = head;
	size_t k = 0;
	while (ptr != nullptr){
		++k;
		ptr = ptr->next;
	}
	return k;
}
double* flist::find(double n){
	Node* ptr = head;
	while (ptr){
		if (ptr->num == n){
			return &(ptr->num);
		}
		ptr = ptr->next;
	}
	return nullptr;
}
const double* flist::find(double n) const{
	Node* ptr = head;
	while (ptr){
		if (ptr->num == n){
			return &(ptr->num);
		}
		ptr = ptr->next;
	}
	return nullptr;
}
double* flist::cycle(){
	Node* slow = head;
	Node* fast = head;
	while (fast && fast->next){
		slow = slow->next;
		fast = fast->next->next;
		if (slow == fast){
			slow = head;
			while (slow != fast){
				slow = slow->next;
				fast = fast->next;
			}
			return &(slow->num);
		}
	}
	return nullptr;
}
const double* flist::cycle() const {
	Node* slow = head;
	Node* fast = head;
	while (fast && fast->next){
		slow = slow->next;
		fast = fast->next->next;
		if (slow == fast){
			slow = head;
			while (slow != fast){
				slow = slow->next;
				fast = fast->next;
			}
			return &(slow->num);
		}
	}
	return nullptr;
}



