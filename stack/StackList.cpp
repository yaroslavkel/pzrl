#include "StackList.h"
#include <stdexcept>
void StackList::push(const ValueType& value){
	_data.push_back(value);
}
void StackList::pop(){
	if (isEmpty()){
		throw std::runtime_error("Stack is empty!");
	}
	_data.pop_back();
}
const ValueType& StackList::top() const {
	if (isEmpty()){
		throw std::runtime_error("Stack is empty!");
	}
	return _data.back();
}
bool StackList::isEmpty() const {
	return _data.empty();
}
size_t StackList::size() const {
	return _data.size();
}
IStackImplementation* StackList::clone() const {
	StackList* newStack = new StackList();
	newStack->_data = List::DoubleLinkedList(this->_data);
	return newStack;
}
