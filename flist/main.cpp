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
}	

