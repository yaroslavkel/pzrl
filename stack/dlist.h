#pragma once
#include <iostream>
namespace List{
class DoubleLinkedList{
    private:
        struct Node{
	    double num;
	    Node* prev;
	    Node* next;
	};
	Node* head = nullptr;
	Node* tail = nullptr;
    public:
        DoubleLinkedList();
	DoubleLinkedList(const DoubleLinkedList& other);
        DoubleLinkedList& operator=(const DoubleLinkedList& other);
	DoubleLinkedList(DoubleLinkedList&& other) noexcept;
	DoubleLinkedList& operator=(DoubleLinkedList&& other) noexcept;
	~DoubleLinkedList();

	void push_front(double n);
	void pop_front();
	void push_back(double n);
	void pop_back();
	void insert(size_t i, double n);
	const double& front() const;
	double& front();
	const double& back() const;
	double& back();
	size_t size() const;
	bool empty() const;
	void display() const;
	void erase(double n);
	void clear();

	double& operator[](unsigned i);
	friend std::ostream& operator<<(std::ostream& strm, const DoubleLinkedList& lst);
        friend DoubleLinkedList operator+(const DoubleLinkedList& one, const DoubleLinkedList& other);
};
}
