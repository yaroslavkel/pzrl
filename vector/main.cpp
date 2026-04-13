#include "vector.h"
#include <iostream>
void print_v(Vector& v){
	for (auto it = v.begin(); it != v.end(); ++it){
		std::cout << *it << " " ;
	}
	std::cout << std::endl;
}
int main()
{
	Vector v;
	v.pushBack(7);
	v.pushFront(7);
	print_v(v);
	return 0;
}
