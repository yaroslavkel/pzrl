#include <iostream>
#include "dlist.h"

namespace List{
DoubleLinkedList::DoubleLinkedList() {}


DoubleLinkedList::DoubleLinkedList(const DoubleLinkedList& other){
    head = nullptr;
    tail = nullptr;
	
    Node* ptr1 = other.head;
 
    while (ptr1) {
        push_back(ptr1->num);
	ptr1 = ptr1->next;
    }
}


DoubleLinkedList& DoubleLinkedList::operator=(const DoubleLinkedList& other){
    if (this == &other){
        return *this;
    }

    clear();
    Node* ptr = other.head;
    while(ptr){
	    push_back(ptr->num);
	    ptr = ptr->next;
    }
    return *this;
}


DoubleLinkedList::DoubleLinkedList(DoubleLinkedList&& other) noexcept{
    head = other.head;
    tail = other.tail;
    other.head = nullptr;
    other.tail = nullptr;
}


DoubleLinkedList& DoubleLinkedList::operator=(DoubleLinkedList&& other) noexcept{
    if (this == &other){
        return *this;
    }
    clear();
    head = other.head;
    tail = other.tail;
    other.head = nullptr;
    other.tail = nullptr;
    return *this;
}


DoubleLinkedList::~DoubleLinkedList(){
    clear();
}


void DoubleLinkedList::push_front(double n) {
    Node* new_node = new Node{n, nullptr, head};
    if (!head){
	head = new_node;
        tail = head;
    }
    else{
        head->prev = new_node;
	head = new_node;
    }

}


void DoubleLinkedList::pop_front() {
    if (head){
        if (tail != head){
            Node* sec_ptr = head->next;
	    delete head;
	    head = sec_ptr;
	    if (head){
		    head->prev = nullptr;
	    }
        }
	else{
	    delete head;
	    head = nullptr;
	    tail = nullptr;
	}
    }
}


void DoubleLinkedList::push_back(double n){
    Node* new_node = new Node{n, tail, nullptr};
    if (tail){
        tail->next = new_node;
	tail = new_node;
    }
    else{
        head = new_node;
	tail = head;
    }
}


void DoubleLinkedList::pop_back(){
    if (tail){
	    if (head == tail){
		    delete tail;
		    head = nullptr;
		    tail = nullptr;
	    }
	    else {
        	Node* prev = tail->prev;
		delete tail;
		tail = prev;
		tail->next = nullptr;
    	    }
    }
}

void DoubleLinkedList::insert(size_t i, double n){
    if (i > size() - 1 || i < 0){
        throw std::runtime_error("index out of range");
    }

    if (i == 0){
        push_front(n);
    }

    else{
        size_t k = 0;
        Node* ptr = head;
        while (k < i){
            ptr = ptr->next;
  	    ++k;
        }
        Node* prev = ptr->prev;
        Node* next = ptr->next;
        Node* new_node = new Node{n, prev, ptr};
        prev->next = new_node;
        }
}


const double& DoubleLinkedList::front() const{
    if (head != nullptr){
	const double& n = head->num;
        return n;
    }
    else{
        throw std::runtime_error("List is empty");
    }
}

double& DoubleLinkedList::front(){
    if (head != nullptr){
	double& n = head->num;
        return n;
    }
    else{
        throw std::runtime_error("List is empty");
    }
}


const double& DoubleLinkedList::back() const{
    if (tail){
	const double& n = tail->num;
        return n;
    }
    else{
        throw std::runtime_error("List is empty");
    }
}

double& DoubleLinkedList::back(){
    if (tail){
	double& n = tail->num;
        return n;
    }
    else{
        throw std::runtime_error("List is empty");
    }
}
size_t DoubleLinkedList::size() const {
    Node* ptr = head;
    unsigned k = 0;
    while (ptr != nullptr){
        ++k;
	ptr = ptr->next;
    }
    return k;
}


bool DoubleLinkedList::empty() const {
    return (head == nullptr) ? true : false;
}


void DoubleLinkedList::display() const {
    Node* ptr = head;
    while (ptr){
	std::cout << ptr->num;
	if (ptr->next){
	    std::cout << " -> ";
	}
	ptr = ptr->next;
    }
    std::cout << std::endl;
}



void DoubleLinkedList::erase(double n){
    Node* ptr = head;
    Node* prev_ptr = head;

    while (ptr != nullptr){
        if (ptr->num == n){
	    Node* next_ptr = ptr->next;
	    if (ptr == head){
		delete head;
	        head = next_ptr;
		if (head){
			head->prev = nullptr;
		}
		ptr = head;
		prev_ptr = head;
	    }
	    else{
	        prev_ptr->next = next_ptr;
		delete ptr;
		ptr = next_ptr;
	    }
	}
	else{
	    prev_ptr = ptr;
	    ptr = ptr->next;
	}
    }
}


void DoubleLinkedList::clear(){
    Node* ptr = head;
    while (ptr != nullptr){
	Node* next_ptr = ptr->next;
	delete ptr;
	ptr = next_ptr;
    }
    head = nullptr;
    tail = nullptr;
}


double& DoubleLinkedList::operator[](unsigned i){
    if (i > size() - 1 || i < 0){
        throw std::runtime_error("index out of range");
    }
    unsigned k = 0;
    Node* ptr = head;
    while (k < i){
        ptr = ptr->next;
	++k;
    }
    double& num = ptr->num;
    return num;
}


std::ostream& operator<<(std::ostream& strm, const DoubleLinkedList& lst){
   unsigned k = 0;
   DoubleLinkedList::Node* ptr = lst.head;
   while (k < lst.size()){
       strm << ptr->num;
       if (k != lst.size() - 1){
           strm << " -> ";
       }
       ptr = ptr->next;
       ++k;
   }
   return strm;
}


DoubleLinkedList operator+(const DoubleLinkedList& one, const DoubleLinkedList& other){
    DoubleLinkedList lst;
    DoubleLinkedList::Node* ptr1 = one.head;
    DoubleLinkedList::Node* ptr2 = lst.head;
    if (ptr1){
        DoubleLinkedList::Node* new_node = new DoubleLinkedList::Node{ptr1->num, nullptr, lst.head};
	lst.head = new_node;
	lst.tail = new_node;
	ptr2 = lst.head;
	while (ptr1->next){
	    ptr1 = ptr1->next;
            DoubleLinkedList::Node* new_node = new DoubleLinkedList::Node{ptr1->num, ptr2, nullptr};
	    ptr2->next = new_node;
	    ptr2 = ptr2->next;
	    lst.tail = ptr2;
	}
    }
    ptr1 = other.head;
    if (ptr1){
        if (!lst.head){
	    DoubleLinkedList::Node* new_node = new DoubleLinkedList::Node{ptr1->num, nullptr, lst.head};
	    lst.head = new_node;
	    lst.tail = new_node;
	    ptr2 = lst.head;
	}
	else{
	    ptr2 = lst.tail;
	}
	while (ptr1){
            DoubleLinkedList::Node* new_node = new DoubleLinkedList::Node{ptr1->num, ptr2, nullptr};
	    ptr1 = ptr1->next;
	    ptr2->next = new_node;
	    ptr2 = ptr2->next;
	    lst.tail = ptr2;
	}
    }
    return lst;

}
}

