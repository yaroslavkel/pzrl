#include "Stack.h"
#include <iostream>

int main() {
    Stack s1;
    s1.push(1.1);
    s1.push(2.2);
    s1.push(3.3);
    
    std::cout << "Top: " << s1.top() << std::endl;
    std::cout << "Size: " << s1.size() << std::endl;
    
    s1.pop();
    std::cout << "After pop, top: " << s1.top() << std::endl;
   
    Stack s2(StackContainer::List);
    s2.push(5.5);
    std::cout << "List stack top: " << s2.top() <
    Stack s3(s1);
    std::cout << "Copied stack top: " << s3.top() << std::endl;
    
    return 0;
}
