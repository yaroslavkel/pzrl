#pragma once
#include "StackImplementation.h"
#include "dlist.h"
class StackList : public IStackImplementation {
	private:
        	List::DoubleLinkedList _data;
	public:
		void push(const ValueType& value) override;
		void pop() override;
		const ValueType& top() const override;
		bool isEmpty() const override;
		size_t size() const override;
		IStackImplementation* clone() const override;
};
