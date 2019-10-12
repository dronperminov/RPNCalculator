#pragma once

#include <iostream>
#include <iomanip>
#include <cmath>
#include <stack>

#include "Parser.hpp"

class Calculator {
	struct Variable {
		std::string name;
		double value;
	};

	struct UserFunction {
		std::string name;
		std::string arg;
		std::vector<std::string> rpn;
	};

	const std::vector<std::string> functions = { "sin", "cos", "tan", "cot", "tg", "ctg", "sh", "sinh", "ch", "cosh", "th", "tanh", "cth", "coth", "sqrt", "exp", "log", "ln", "abs" };
	
	std::vector<Variable> variables;
	std::vector<UserFunction> userFunctions;

	bool IsOperator(const std::string &s) const;
	bool IsFunction(const std::string &s) const;
	bool IsUserFunction(const std::string &s) const;
	bool IsNumber(const std::string &s) const;
	bool IsVariable(const std::string &s) const;

	int GetPriority(const std::string &s) const;

	std::vector<std::string> GetRPN(const std::vector<std::string> &lexemes) const;

	int GetVariableByName(const std::string &name) const;
	int GetUserFunctionByName(const std::string &name) const;

	double CalculateOperator(const std::string &op, double arg1, double arg2) const;
	double CalculateFunction(const std::string &name, double arg) const;
	double CalculateUserFunction(const std::string &name, double arg) const;
	double Calculate(const std::vector<std::string> &rpn) const;

	void ProcessSet(std::vector<std::string> &lexemes);
	void ProcessDef(std::vector<std::string> &lexemes);

public:
	void ProcessCommand(const std::string cmd);
	void PrintState() const;
};

bool Calculator::IsOperator(const std::string &s) const {
	return s == "+" || s == "-" || s == "*" || s == "/" || s == "^" || s == "%";
}

bool Calculator::IsFunction(const std::string &s) const {
	for (size_t i = 0; i < functions.size(); i++)
		if (s == functions[i])
			return true;

	return false;
}

bool Calculator::IsUserFunction(const std::string &s) const {
	for (size_t i = 0; i < userFunctions.size(); i++)
		if (s == userFunctions[i].name)
			return true;

	return false;
}

bool Calculator::IsNumber(const std::string &s) const {
	for (size_t i = 0; i < s.length(); i++)
		if (s[i] != '.' && (s[i] < '0' || s [i] > '9'))
			return false;

	return true;
}

bool Calculator::IsVariable(const std::string &s) const {
	if (IsFunction(s) || IsUserFunction(s))
		return false;

	for (size_t i = 0; i < s.length(); i++)
		if (!((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || (i != 0 && s[i] >= '0' && s[i] <= '9')))
			return false;
	
	return true;
}

int Calculator::GetPriority(const std::string &s) const {
	if (s == "!")
		return 3;

	if (s == "^")
		return 4;

	if (s == "*" || s == "/")
		return 2;


	if (s == "+" || s == "-")
		return 1;

	return 0;
}

std::vector<std::string> Calculator::GetRPN(const std::vector<std::string> &lexemes) const {
	std::stack<std::string> stack;
	std::vector<std::string> rpn;

	bool mayUnary = true;

	for (size_t i = 0; i < lexemes.size(); i++) {
		if (IsNumber(lexemes[i]) || IsVariable(lexemes[i])) {
			rpn.push_back(lexemes[i]);
			mayUnary = false;
		}
		else if (IsFunction(lexemes[i]) || IsUserFunction(lexemes[i])) {
			stack.push(lexemes[i]);
			mayUnary = true;
		}
		else if (lexemes[i] == "(") {
			stack.push(lexemes[i]);
			mayUnary = true;
		}
		else if (lexemes[i] == ")") {
			while (!stack.empty() && stack.top() != "(") {
				rpn.push_back(stack.top());
				stack.pop();
			}

			if (stack.empty())
				throw std::string("Incorrect expression");

			stack.pop();
			mayUnary = false;
		}
		else if (IsOperator(lexemes[i])) {
			std::string op = lexemes[i];

			if (op == "-" && mayUnary)
				op = "!"; // унарный минус

			while (!stack.empty() && (IsFunction(stack.top()) || IsUserFunction(stack.top()) || GetPriority(stack.top()) >= GetPriority(op))) {
				rpn.push_back(stack.top());
				stack.pop();
			}

			stack.push(op);
			mayUnary = false;
		}
	}

	while (!stack.empty()) {
		if (!IsOperator(stack.top()) && !IsFunction(stack.top()) && !IsUserFunction(stack.top()) && stack.top() != "!")
			throw std::string("Incorrect expression: brackets are disbalanced");

		rpn.push_back(stack.top());
		stack.pop();
	}

	return rpn;
}

int Calculator::GetVariableByName(const std::string &name) const {
	for (size_t i = 0; i < variables.size(); i++)
		if (variables[i].name == name)
			return i;

	return -1;
}

int Calculator::GetUserFunctionByName(const std::string &name) const {
	for (size_t i = 0; i < userFunctions.size(); i++)
		if (userFunctions[i].name == name)
			return i;

	return -1;
}

double Calculator::CalculateOperator(const std::string &op, double arg1, double arg2) const {
	if (op == "+")
		return arg1 + arg2;

	if (op == "-")
		return arg1 - arg2;

	if (op == "*")
		return arg1 * arg2;

	if (op == "/" || op == "%") {
		if (arg2 == 0)
			throw std::string("Division by zero");

		return op == "/" ? arg1 / arg2 : fmod(arg1, arg2);
	}

	if (op == "^")
		return pow(arg1, arg2);

	throw std::string("Unknown operator");
}

double Calculator::CalculateFunction(const std::string &name, double arg) const {
	if (name == "sin")
		return sin(arg);

	if (name == "cos")
		return cos(arg);

	if (name == "tan" || name == "tg")
		return tan(arg);

	if (name == "cot" || name == "ctg")
		return 1.0 / tan(arg);

	if (name == "sinh" || name == "sh")
		return sinh(arg);

	if (name == "cosh" || name == "ch")
		return cosh(arg);

	if (name == "tanh" || name == "th")
		return tanh(arg);

	if (name == "coth" || name == "cth")
		return 1.0 / tanh(arg);

	if (name == "sqrt")
		return sqrt(arg);

	if (name == "exp")
		return exp(arg);

	if (name == "log")
		return log(arg) / log(10);

	if (name == "ln")
		return log(arg);

	if (name == "abs")
		return fabs(arg);

	throw std::string("Unknown function");
}

double Calculator::CalculateUserFunction(const std::string &name, double arg) const {
	int index = GetUserFunctionByName(name);

	if (index == -1)
		throw std::string("Unknown user function '") + name + "'";

	std::vector<std::string> rpn = userFunctions[index].rpn;

	for (size_t i = 0; i < rpn.size(); i++)
		if (rpn[i] == userFunctions[index].arg)
			rpn[i] = std::to_string(arg);

	return Calculate(rpn);
}

double Calculator::Calculate(const std::vector<std::string> &rpn) const {
	std::stack<double> stack;

	for (size_t i = 0; i < rpn.size(); i++) {
		if (IsOperator(rpn[i])) {
			if (stack.size() < 2)
				throw std::string("Incorrect expression");

			double arg2 = stack.top();
			stack.pop();
			double arg1 = stack.top();
			stack.pop();

			stack.push(CalculateOperator(rpn[i], arg1, arg2));
		}
		else if (IsFunction(rpn[i])) {
			if (stack.size() < 1)
				throw std::string("Incorrect expression");

			double arg = stack.top();
			stack.pop();
			stack.push(CalculateFunction(rpn[i], arg));
		}
		else if (IsUserFunction(rpn[i])) {
			if (stack.size() < 1)
				throw std::string("Incorrect expression");

			double arg = stack.top();
			stack.pop();
			stack.push(CalculateUserFunction(rpn[i], arg));
		}
		else if (rpn[i] == "!") {
			if (stack.size() < 1)
				throw std::string("Incorrect expression");

			double arg = stack.top();
			stack.pop();

			stack.push(-arg);
		}
		else if (IsVariable(rpn[i])) {
			int index = GetVariableByName(rpn[i]);

			if (index == -1)
				throw std::string("Variable '") + rpn[i] + "' is not set";

			stack.push(variables[index].value);
		}
		else if (IsNumber(rpn[i]) || (rpn[i][0] == '-' && IsNumber(rpn[i].substr(1)))) {
			stack.push(std::stod(rpn[i]));
		}
		else
			throw std::string("Unknown rpn lexeme '") + rpn[i] + "'";
	}

	if (stack.size() != 1)
		throw std::string("Incorrect expression");

	return stack.top();
}

void Calculator::ProcessSet(std::vector<std::string> &lexemes) {
	if (lexemes.size() < 3)
			throw std::string("Incorrect set instruction");

		if (!IsVariable(lexemes[1]))
			throw std::string("Second argument must be variable value");

		std::string name = lexemes[1];
		lexemes.erase(lexemes.begin(), lexemes.begin() + 2);
		std::vector<std::string> rpn = GetRPN(lexemes);
		double value = Calculate(rpn);

		int index = GetVariableByName(name);

		if (index == -1) {
			variables.push_back({ name, value });
		}
		else {
			variables[index].value = value;
		}
}

void Calculator::ProcessDef(std::vector<std::string> &lexemes) {
	if (lexemes.size() < 6)
		throw std::string("Incorrect def instruction");

	if (lexemes[2] != "(" || !IsVariable(lexemes[3]) || lexemes[4] != ")")
		throw std::string("Incorrect argument definition");

	std::string name = lexemes[1];
	std::string arg = lexemes[3];

	lexemes.erase(lexemes.begin(), lexemes.begin() + 5);
	std::vector<std::string> rpn = GetRPN(lexemes);

	int index = GetUserFunctionByName(name);

	if (index == -1) {
		userFunctions.push_back({ name, arg, rpn });
	}
	else {
		userFunctions[index] = { name, arg, rpn };
	}
}

void Calculator::ProcessCommand(const std::string cmd) {
	Parser parser(cmd);
	std::vector<std::string> lexemes = parser.Parse();

	if (lexemes[0] == "set") {
		ProcessSet(lexemes);
	}
	else if (lexemes[0] == "def") { // def name(arg) expr
		ProcessDef(lexemes);
	}
	else {
		std::vector<std::string> rpn = GetRPN(lexemes);

		double value = Calculate(rpn);
		std::cout << value << std::endl;
	}
}

void Calculator::PrintState() const {
	if (variables.size() == 0) {
		std::cout << "No variables" << std::endl;
	}
	else {
		std::cout << "+--------------------------------------+" << std::endl;
		std::cout << "|            User variables            |" << std::endl;
		std::cout << "+--------------+-----------------------+" << std::endl;
		std::cout << "|     name     |         value         |" << std::endl;
		std::cout << "+--------------+-----------------------+" << std::endl;

		for (size_t i = 0; i < variables.size(); i++)
			std::cout << "| " << std::setw(12) << variables[i].name << " | " << std::setw(21) << variables[i].value << " |" << std::endl;

		std::cout << "+--------------+-----------------------+" << std::endl;
	}

	if (userFunctions.size() == 0) {
		std::cout << "No user functions" << std::endl;
	}
	else {
		std::cout << "+--------------------------------------+" << std::endl;
		std::cout << "|            User functions            |" << std::endl;
		std::cout << "+--------------+-----------------------+" << std::endl;
		std::cout << "|     name     |          arg          |" << std::endl;
		std::cout << "+--------------+-----------------------+" << std::endl;

		for (size_t i = 0; i < userFunctions.size(); i++)
			std::cout << "| " << std::setw(12) << userFunctions[i].name << " | " << std::setw(21) << userFunctions[i].arg << " |" << std::endl;

		std::cout << "+--------------+-----------------------+" << std::endl;
	}
}