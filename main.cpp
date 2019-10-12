#include <iostream>
#include "Parser.hpp"
#include "Calculator.hpp"

using namespace std;

void Help() {
	cout << "Type command after '>' character." << endl;
	cout << "Types of commands: " << endl;
	cout << "- set:" << endl;
	cout << "    syntax: set variable_name variable_value" << endl;
	cout << "    variable_name - name of variable for set" << endl;
	cout << "    variable_value - computable expression" << endl;
	cout << "    example: set a (1+2)*sqrt(5)" << endl;
	cout << endl;
	cout << "- def:" << endl;
	cout << "    syntax: def function_name (argument_name) function_body" << endl;
	cout << "    function_name - name of function to difine" << endl;
	cout << "    argument_name - name of argument" << endl;
	cout << "    function_body - computable expression" << endl;
	cout << "    example: def f1(x) sin(x) + cos(x) - 5" << endl;
	cout << endl;
	cout << "- expression:" << endl;
	cout << "    example: 5 + 2 * f1(a) - 5" << endl;
	cout << endl;
	cout << "- state: " << endl;
	cout << "    print current variables and user functions" << endl;
	cout << endl;
	cout << "- quit" << endl;
	cout << "    quit this application" << endl;
	cout << endl;
}

int main() {
	Calculator calculator;
	string expr;
	
	cout << "Welcome to the calculator application" << endl;
	cout << "For print instruction type help" << endl;

	while (true) {
		cout << ">";
		getline(cin, expr);

		if (expr == "quit")
			break;

		if (expr == "state") {
			calculator.PrintState();
		}
		else if (expr == "help") {
			Help();
		}
		else {
			try {
				calculator.ProcessCommand(expr);
			}
			catch (string s) {
				cout << "Error: " << s << endl;
			}
		}
	}
}