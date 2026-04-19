#pragma once
#include "StackImplementation.h"
#include <vector>
class StackVector : public IStackImplementation {
	private:
		std::vector<ValueType> _data;
	public:
		void push(const ValueType& value) override;
		void pop() override;
		const ValueType& top() const override;
		bool isEmpty() const override;
		size_t size() const override;
		IStackImplementation* clone() const override;
};
	
