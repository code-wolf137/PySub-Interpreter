#ifndef EXPEVALUATOR_H
#define EXPEVALUATOR_H

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <stack>
#include <queue>
#include "interface.h"
#include "lexanalyzer.h"

using namespace std;

class expEvaluator
{
public: 

	Lexical_Analyzer lexEval;

	void infixToPostfix(string input)
	{
		lexEval.startLexAnalysis(input, 0);
        vector<pair<string, Lexical_Analyzer::categoryType>> inputTokens = lexEval.sendData(0);

		stack<pair<string, Lexical_Analyzer::categoryType>> tempStack;
		vector<pair<string, Lexical_Analyzer::categoryType>> output;

		for (int i = 0; i < inputTokens.size(); i++)
		{
			if (inputTokens[i].second == Lexical_Analyzer::categoryType::NUMERIC_LITERAL)
			{
				output.push_back(inputTokens[i]);
			}
			else if (inputTokens[i].second == Lexical_Analyzer::categoryType::LEFT_PAREN)
			{
				tempStack.push(inputTokens[i]);
				continue;
			}
			else if (inputTokens[i].second == Lexical_Analyzer::categoryType::RIGHT_PAREN)
			{
				if (tempStack.empty())
				{
					cout << "Error. Input not a valid expression." << endl;
					return;
				}
				while (true)
				{
					if (tempStack.top().second == Lexical_Analyzer::categoryType::LEFT_PAREN)
					{
						tempStack.pop();
						break;
					}
					output.push_back(tempStack.top());
					tempStack.pop();
				}
				
			}
			else if (inputTokens[i].second == Lexical_Analyzer::categoryType::ARITH_OP ||
					inputTokens[i].second == Lexical_Analyzer::categoryType::RELATIONAL_OP || 
					inputTokens[i].second == Lexical_Analyzer::categoryType::LOGICAL_OP)
			{
				
				while (!tempStack.empty() && tempStack.top().second != Lexical_Analyzer::categoryType::LEFT_PAREN &&
					is_higher_precedence(tempStack.top(), inputTokens[i]))
				{
					output.push_back(tempStack.top());
					tempStack.pop();
				}
				
				tempStack.push(inputTokens[i]);

			}
		}

		while (!tempStack.empty())
		{
			output.push_back(tempStack.top());
			tempStack.pop();
		}

		for (int i = 0; i < output.size(); i++)
			cout << output[i].first << " ";
		cout << endl;

		cout << postfixEval(output) << endl;

		lexEval.clear();
	}

	int postfixEval(vector<pair<string, Lexical_Analyzer::categoryType>> tokens)
	{
		stack<int> evalStack;
		int result, variable1, variable2;

		for (int i = 0; i < tokens.size(); i++)
		{
			if (tokens[i].second == Lexical_Analyzer::categoryType::NUMERIC_LITERAL)
				evalStack.push(stod(tokens[i].first));
			else if (tokens[i].first == "not") //unary operator
			{
				variable1 = evalStack.top();
				if (variable1 == 1)
					evalStack.push(0);
				else if (variable1 == 1)
					evalStack.push(1);
				else {
					cout << "Error not a valid expression." << endl;
					exit(1);
				}
			}
			else
			{
				variable2 = evalStack.top();
				evalStack.pop();
				variable1 = evalStack.top();
				evalStack.pop();

				evalStack.push(variableEval(variable1, variable2, tokens[i].first));
			}
		}
		
		return evalStack.top();
	}

	int variableEval(int x, int y, string op)
	{
		if (op == "*")
			return x * y;
		else if (op == "/")
			return x / y;
		else if (op == "%")
			return x % y;
		else if (op == "+")
			return x + y;
		else if (op == "-")
			return x - y;
		else if (op == "<")
		{
			if (x < y)
				return 1;
			else
				return 0;
		}
		else if (op == "<=")
		{
			if (x <= y)
				return 1;
			else
				return 0;
		}
		else if (op == ">")
		{
			if (x > y)
				return 1;
			else
				return 0;
		}
		else if (op == ">=")
		{
			if (x >= y)
				return 1;
			else
				return 0;
		}
		else if (op == "!=")
		{
			if (x != y)
				return 1;
			else
				return 0;
		}
		else if (op == "==")
		{
			if (x == y)
				return 1;
			else
				return 0;
		}
		else if (op == "and")
		{
			if (x ==1 && y == 1)
				return 1;
			else
				return 0;
		}
		else if (op == "or")
		{
			if (x == 1 || y == 1)
				return 1;
			else
				return 0;
		}
	}


	int findPrecedence(pair<string, Lexical_Analyzer::categoryType> token)
	{
		if (token.first == "%" || token.first == "*" || token.first == "/")
			return 5;
		else if (token.first == "+" || token.first == "-")
			return 4;
		else if (token.second == Lexical_Analyzer::categoryType::RELATIONAL_OP)
			return 3;
		else if (token.first == "not")
			return 2;
		else if (token.first == "and")
			return 2;
		else if (token.first == "or")
			return 1;
					
	}

	bool is_higher_precedence(pair<string, Lexical_Analyzer::categoryType> scanned, pair<string, Lexical_Analyzer::categoryType> stackTop) //returns true if equal or higher precedence
	{
		if (findPrecedence(scanned) >= findPrecedence(stackTop))
			return true;
		else
			return false;
	}

	int getValue(string key)
	{
		if (symbolTable.count(key))
		{
			return stod(symbolTable[key]); //returns integer value of the value for the variable
		}
		else
		{
			cout << "Error not found." << endl;
			exit(0);
		}
	}

	void addValue(string key, string value)
	{
		symbolTable[key] = value;
		return;
	}

	void clear()
	{
		symbolTable.clear();
	}

private:

	typedef map<string, string> symbolTableType;
	symbolTableType symbolTable;
};


#endif