#pragma once

#include <iostream>
#include <string>
#include <vector>

class Parser {
	std::string expr; // строка выражения

	bool IsOperator(char c) const;
	bool IsBracket(char c) const;
	bool IsLetter(char c) const;
	bool IsDigit(char c) const;
	bool IsLetterOrDigit(char c) const;
	
public:
	Parser(const std::string &expr);

	std::vector<std::string> Parse();
};

bool Parser::IsOperator(char c) const {
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%';
}

bool Parser::IsBracket(char c) const {
	return c == '(' || c == ')';
}

bool Parser::IsLetter(char c) const {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Parser::IsDigit(char c) const {
	return c >= '0' && c <= '9';
}

bool Parser::IsLetterOrDigit(char c) const {
	return IsLetter(c) || IsDigit(c);
}

Parser::Parser(const std::string &expr) {
	this->expr = expr;
}

std::vector<std::string> Parser::Parse() {
	std::vector<std::string> lexemes;
	size_t i = 0;

	while (i < expr.length()) {
		if (IsOperator(expr[i]) || IsBracket(expr[i]) || expr[i] == ',') {
			lexemes.push_back(std::string(1, expr[i]));
			i++;
		}
		else if (IsLetter(expr[i])) {
			std::string identifier = "";

			while (i < expr.length() && IsLetterOrDigit(expr[i])) {
				identifier += expr[i];
				i++;
			}

			lexemes.push_back(identifier);
		}
		else if (IsDigit(expr[i])) {
			std::string number = "";
			int points = 0;

			while (i < expr.length() && (IsDigit(expr[i]) || expr[i] == '.')) {
				if (expr[i] == '.') {
					points++;

					if (points > 1)
						throw std::string("Invalid float number");
				}

				number += expr[i];
				i++;
			}

			lexemes.push_back(number);
		}
		else if (expr[i] == ' ') {
			i++;
		}
		else
			throw std::string("Unknown character");
	}

	if (lexemes.size() == 0)
		throw std::string("Expression is empty");

	return lexemes;
}