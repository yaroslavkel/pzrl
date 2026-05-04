#pragma once
#include <iostream>
#include <utility>
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
		std::pair<Node*, size_t> cycle();
		std::pair<const Node*, size_t> cycle() const;
		void create_cycle(size_t idx);
};
