#include "StackVector.h"
#include <stdexcept>
void StackVector::push(const ValueType& value){
	_data.pushBack(value);
}
void StackVector::pop(){
	if (isEmpty()){
		throw std::runtime_error("Stack is empty!");
	}
	_data.popBack();
}
const ValueType& StackVector::top() const {
	if (isEmpty()){
		throw std::runtime_error("Stack is empty!");
	}
	return _data[_data.size() - 1];
}
bool StackVector::isEmpty() const {
	return _data.size() == 0;
}
size_t StackVector::size() const {
	return _data.size();
}
IStackImplementation* StackVector::clone() const {
	StackVector* newStack = new StackVector();
	for (size_t i = 0; i < _data.size(); i++){
		newStack->_data.pushBack(_data[i]);
	}
	return newStack;
}

