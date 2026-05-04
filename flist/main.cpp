#include "flist.h"
#include <iostream>
int main(){
	flist list = flist();
	std::cout << "Current list length: " << list.size() << std::endl;
	list.push_back(6);
	list.push_back(7);
	list.push_back(6.7);
	list.display();
	double* finder = list.find(7);
	std::cout << "Current list length: " << list.size() << std::endl;
	std::cout << "Adress of 7: " << finder << std::endl;
	list.pop_back();
	list.display();
	auto result = list.cycle();
	if (result.first == nullptr){
		std::cout << "There is no cycle!" << std::endl;
	}
	else {
		std::cout << "Somehow there is a cycle" << std::endl;
	}
	flist cycle_list;
	cycle_list.push_back(10);
	cycle_list.push_back(20);
	cycle_list.push_back(30);
	cycle_list.push_back(40);
	cycle_list.push_back(50);
	cycle_list.display();
	cycle_list.create_cycle(1);
	auto cycle_result = cycle_list.cycle();
	if (cycle_result.first == nullptr){
		std::cout << " Somehow there is no cycle!" << std::endl;
	}
	else {
		std::cout << "There is a cycle!" << std::endl;
	}
	std::cout << "Start of the cycle: " << cycle_result.first->num << " Length: " << cycle_result.second << std::endl;
}

