#pragma once
#include <cstddef>
class flist {
	private:
		struct Node{
			Node* next;
			double num;
		};
		Node* head = nullptr;
	public:
		flist();
		~flist();
		void push_back(double n);
		void pop_back();
		size_t size() const;
		double* find(double n);
		const double* find(double n) const;
		void display() const;
		bool isEmpty() const;
		const double& top() const;
};
