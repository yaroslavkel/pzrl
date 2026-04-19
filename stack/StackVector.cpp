#include "StackVector.h"
#include <stdexcept>
void StackVector::push(const ValueType& value){
	_data.push_back(value);
}
void StackVector::pop(){
	if (isEmpty()){
		throw std::runtime_error("Stack is empty!");
	}
	_data.pop_back();
}
const ValueType& StackVector::top() const {
	if (isEmpty()){
		throw std::runtime_error("Stack is empty!");
	}
	return _data.back();
}
bool StackVector::isEmpty() const {
	return _data.empty();
}
size_t StackVector::size() const {
	return _data.size();
}
IStackImplementation* StackVector::clone() const {
	StackVector* newStack = new StackVector();
	newStack->_data = this->_data;
	return newStack;
}

