#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <sstream>
std::string preprocess(const std::string& input){
	std::string result;
	bool prev = true;
	for (size_t i = 0; i < input.length(); i++){
		char c = input[i];
		if (c == ' '){
			result += c;
			continue;
		}
		if (c == '-' && prev){
			result += "0 - ";
			prev = true;
		}
		else {
			result += c;
			if (c == '+'|| c == '-' || c == '*' || c == '/' || c == '('){
				prev = true;
			}
			else if (c == ')' || (c >= '0' && c <= '9')){
				prev = false;
			}
		}
	}
	return result;
}
int precedence(char op){
	if (op == '-' || op == '+'){
		return 1;
	}
	else if (op == '*' || op == '/'){
		return 2;
	}
	return 0;
}
bool isNumber(const std::string& token){
	try {
		std::stod(token);
		return true;
	}
	catch(...){
		return false;
	}
}
bool isOperator(char op){
	return op == '+' || op == '-' || op == '/' || op == '*';
}
void shunting(const std::string& input, std::queue<std::string>& queueOut, std::stack<char>& stackOp){
	std::istringstream iss(input);
	std::string token;
	while (iss >> token){
		if (isNumber(token)){
			queueOut.push(token);
		}
		else if (isOperator(token[0])){
			char op = token[0];
			while (!stackOp.empty() && stackOp.top() != '(' && precedence(stackOp.top()) >= precedence(op)){
				queueOut.push(std::string(1, stackOp.top()));
				stackOp.pop();
			}
			stackOp.push(op);
		}
		else if (token[0] == '('){
			stackOp.push('(');
		}
		else if (token[0] == ')'){
			while (!stackOp.empty() && stackOp.top() != '('){
				queueOut.push(std::string(1, stackOp.top()));
				stackOp.pop();
			}
			if (!stackOp.empty()){
				stackOp.pop();
			}
		}
	}
	while (!stackOp.empty()){
		queueOut.push(std::string(1, stackOp.top()));
		stackOp.pop();
	}
}
double evaluate(std::queue<std::string>& queueOut){
	std::stack<double> evalStack;
	while (!queueOut.empty()){
		std::string token = queueOut.front();
		queueOut.pop();
		if (isNumber(token)){
			evalStack.push(std::stod(token));
		}
		else if (isOperator(token[0])){
			double b = evalStack.top();
			evalStack.pop();
			double a = evalStack.top();
			evalStack.pop();
			double result = 0;
			if (token[0] == '+'){
				result = a + b;
			}
			else if (token[0] == '-'){
				result = a - b;
			}
			else if (token[0] == '*'){
				result = a * b;
			}
			else if (token[0] == '/'){
				if (b != 0){
					result = a / b;
				}
				else {
					throw std::invalid_argument("Division by zero!");
				}
			}
			evalStack.push(result);
		}
	}
	return evalStack.top();		
}
int main(int argc, char* argv[]){
	if (argc < 2){
		std::cout << "Wrong usage! " << std::endl;
		return 1;
	}
	std::string input_string;
	for (int i = 1; i < argc; i++){
		input_string += argv[i];
		input_string += ' ';
	}
	input_string = preprocess(input_string);
	std::cout << "Preprocessed: " << input_string << std::endl;
	std::queue<std::string> queueOut;
	std::stack<char> stackOp;
	shunting(input_string, queueOut, stackOp);
	std::cout << "Postfix: ";
	std::queue<std::string> copy = queueOut;
	while (!copy.empty()){
		std::cout << copy.front() << ' ';
		copy.pop();
	}
	std::cout << std::endl;
	double result = evaluate(queueOut);
	std::cout << "Result: " << result << std::endl;
	return 0;
}





