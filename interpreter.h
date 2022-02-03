#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stack>
#include "interface.h"
#include "lexanalyzer.h"
#include "expevaluator.h"

using namespace std;

class Interpreter {

	friend class Interface;

public:

	
	expEvaluator eval;

	void run(vector<string> programCodeLines)
	{
		for (int i = 0; i < programCodeLines.size(); i++) {
			//cout << programCodeLines[i] << endl;
			interpretCode(programCodeLines[i]);
		}
		if_loop = false;
		if_executed = false;
		for (int i = 0; i < ifStatement.size(); i++)
		{
			interpretCode(ifStatement[i]);
		}
		ifStatement.clear();
		loop_index = 0;
		loop_indent = "";
		return;
	}

	void interpretCode(string codeLine)
	{
		Lexical_Analyzer lex;
		lex.startLexAnalysis(codeLine, 0); //analyzes codeLine
		int index = 0;
		string value;

		if (failedCondition) //condition for loop or if not being executed
		{
			if (lex.tokenInfo[0].empty()) //fail 
			{
				loop_index = 0;
				failedCondition = false;
				loop_indent = "";
			}
			else if (lex.tokenInfo[0][0].first == "elif")
			{
				loop_index = 0;
				failedCondition = false;
				loop_indent = "";
			}
			else if (lex.tokenInfo[0][0].first == "else")
			{
				loop_index = 0;
				newIndent = true;
				failedCondition = false;
				if_loop = true;
				return;
			} 
			else if (loop_index == 0) //if first line in block
			{
				loop_indent = lex.tokenInfo[0][0].first;
				loop_index++;
				return;
			}
			else if (loop_indent != lex.tokenInfo[0][0].first) //fail 
			{
				loop_index = 0;
				failedCondition = false;
				loop_indent = "";
			}
			else if (loop_indent == lex.tokenInfo[0][0].first)
			{
				loop_index++;
				return;
			}
		}

		if (if_executed)
		{
			if (lex.tokenInfo[0].empty());
			else if (newIndent)
			{
				if (lex.tokenInfo[0][0].second != Lexical_Analyzer::categoryType::INDENT)
				{
					cout << "... Syntax error no indent before code" << endl;
					return;
				}
				loop_indent = lex.tokenInfo[0][0].first;
				newIndent = false;
				return;
			}
			else if (lex.tokenInfo[0][0].first == loop_indent)
			{
				return;
			}
			else if (lex.tokenInfo[0][index].first == "elif" || lex.tokenInfo[0][index].first == "else")
			{
				newIndent = true;
				return;
			}
		}
		
		if (if_loop) //adding to block of if statements
		{
			if (lex.tokenInfo[0].empty())
			{
				if_loop = false;
				if_executed = false;
				for (int i = 0; i < ifStatement.size(); i++)
				{
					interpretCode(ifStatement[i]);
				}
				ifStatement.clear();
				loop_index = 0;
				loop_indent = "";
				return;
			}
			if (loop_index == 0)
			{
				if (lex.tokenInfo[0][0].second != Lexical_Analyzer::categoryType::INDENT)
				{
					cout << "... Syntax error no indent before code." << endl;
					return;
				}
				loop_indent = lex.tokenInfo[0][0].first;
				for (int i = 0; i < lex.tokenInfo[0].size(); i++)
				{
					if (lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::UNKNOWN || (lex.tokenInfo[0].size() == 2 && lex.tokenInfo[0][0].first != "else")) //breaks
					{
						cout << "Error invalid syntax" << endl << endl;
						ifStatement.clear();
						loop_index = 0;
						if_executed = true;
						if_loop = false;
						loop_indent = "";
						return;
					}
				}
				ifStatement.push_back(codeLine); //adds to vector
				loop_index++;
				return;
			}
			else if (lex.tokenInfo[0][0].first == "elif" || lex.tokenInfo[0][0].first == "else")
			{
				if_executed = true;
				newIndent = true;
				return;
			}
			else
			{
				ifStatement.push_back(codeLine);
				loop_index++;
				return;

				if (lex.tokenInfo[0].empty() || lex.tokenInfo[0][0].first != loop_indent)
				{
					
				}
				else
				{
					
				}
			}
		}

		if (while_loop)
		{
			if (loop_index == 0)
			{
				if (lex.tokenInfo[0][0].second != Lexical_Analyzer::categoryType::INDENT)
				{
					cout << "... Syntax error no indent before code." << endl;
					return;
				}
				loop_indent = lex.tokenInfo[0][0].first;
				for (int i = 0; i < lex.tokenInfo[0].size(); i++)
				{
					if (lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::UNKNOWN || lex.tokenInfo[0].size() == 2)
					{
						cout << "Error invalid syntax" << endl << endl;
						whileLoop.clear();
						while_loop = false;
						loop_index = 0;
						whileCondition = "";
						return;
					}
				}
				whileLoop.push_back(codeLine);
				loop_index++;
				return;
			}
			else
			{
				if (lex.tokenInfo[0].empty() || lex.tokenInfo[0][0].first != loop_indent)
				{
					while_loop = false;
					while (eval.infixToPostfix(whileCondition))
					{
						for (int i = 0; i < whileLoop.size(); i++)
						{
							interpretCode(whileLoop[i]);
						}
					}
					whileLoop.clear();
					loop_index = 0;
					whileCondition = "";
					loop_indent = "";
					return;
				}
				else if (lex.tokenInfo[0][0].first == loop_indent)
				{
					whileLoop.push_back(codeLine);
					loop_index++;
					return;
				}
			}
		}

		if (lex.tokenInfo.empty() || lex.tokenInfo[0].empty())
			return;
		else if (lex.tokenInfo[0][0].second == Lexical_Analyzer::categoryType::INDENT)
			index++;

		


		if (lex.tokenInfo[0][index].second == Lexical_Analyzer::categoryType::COMMENT) //if comment, ignore
		{
			return;
		}
		else if (lex.tokenInfo[0].size() < 2)
		{
			cout << "Syntax error please revise" << endl << endl;
			return;
		}
		else if (lex.tokenInfo[0][index + 1].second == Lexical_Analyzer::categoryType::ASSIGNMENT_OP) //if assignment statement
		{
			if (lex.tokenInfo[0].size() == 3 + index)
				value = to_string(eval.infixToPostfix(lex.tokenInfo[0][index + 2].first));
			else if (lex.tokenInfo[0][index + 2].first == "int" && lex.tokenInfo[0][index + 4].first == "input")
			{
				if (lex.tokenInfo[0].size() < 9)
				{
					cout << "Error invalid code." << endl;
					exit(1);
				}
				for (int j = 1; j < lex.tokenInfo[0][index + 6].first.length() - 1; j++)
					cout << lex.tokenInfo[0][index + 6].first[j];
				cout << " ";
				getline(cin, value);
			}
			else
			{
				string expressionInput = codeLine;
				string::iterator iter = expressionInput.begin();
				while (*iter != '=')
				{
					iter++;
				}
				expressionInput.erase(expressionInput.begin(), ++iter);
				value = to_string(eval.infixToPostfix(expressionInput));
			}
			eval.addValue(lex.tokenInfo[0][index].first, value);
		}
		else if (lex.tokenInfo[0][index].first == "if" || lex.tokenInfo[0][index].first == "elif" || lex.tokenInfo[0][index].first == "else") //if statement
		{
			/*if (lex.tokenInfo[0][index].first == "else")
			{
				if_loop == true;
				return;
			}*/
			string condition = "";
			int k = index + 1;

			if (lex.tokenInfo[0][lex.tokenInfo[0].size() - 1].first != ":")
			{
				for (int i = 0; i < lex.tokenInfo[0].size(); i++)
					cout << lex.tokenInfo[0][i].first;
				cout << "Invalid syntax. Please make sure that your condition statement ends with ':'" << endl;
				return;
			}

			if (lex.tokenInfo[0][index].first == "else")
			{
				newIndent = true;
				if_executed = true;
				return;
			}

			while (lex.tokenInfo[0][k].first != ":") //loops through to get condition
			{
				condition += lex.tokenInfo[0][k].first;
				k++;
			}

			
			if (eval.infixToPostfix(condition)) //if condition evaluates to true
			{
				if_loop = true;
				return;
			}
			else
			{

				failedCondition = true;
				loop_index = 0;
				return;
			}
			
		}
		else if (lex.tokenInfo[0][index].first == "while") //while loop
		{
			whileCondition = "";
			int k = index + 1;
			if (lex.tokenInfo[0][lex.tokenInfo[0].size() - 1].first != ":")
			{
				cout << "Invalid syntax. Please make sure that your condition statement ends with ':'" << endl;
				return;
			}
			while (lex.tokenInfo[0][k].first != ":") //loops through to get condition
			{
				whileCondition += lex.tokenInfo[0][k].first;
				k++;
			}
			if (eval.infixToPostfix(whileCondition)) //if condition evaluates to true
			{
				while_loop = true;
				return;
			}
			else
				failedCondition = true;
		}
		else if (lex.tokenInfo[0][index].first == "print") //if print statement
		{
			if (lex.tokenInfo[0].size() < 4)
			{
				cout << "Syntax error please revise" << endl << endl;
				return;
			}

			for (int i = 0; i < lex.tokenInfo[0].size(); i++)
			{
				if (lex.tokenInfo[0][i].first == ")") {
					cout << endl;
					return;
				}
				else if (lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::STRING_LITERAL)
				{
					for (int j = 1; j < lex.tokenInfo[0][i].first.length() - 1; j++)
						cout << lex.tokenInfo[0][i].first[j];
				}
				else if (lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::NUMERIC_LITERAL)
				{
					if (lex.tokenInfo[0][i + 1].second == Lexical_Analyzer::categoryType::COMMA)
						cout << lex.tokenInfo[0][i].first;
					else
					{
						string valueinput = "";
						while (lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::IDENTIFIER || lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::ASSIGNMENT_OP
							|| lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::RELATIONAL_OP || lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::NUMERIC_LITERAL
							|| lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::LOGICAL_OP || lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::ARITH_OP)
						{
							valueinput += lex.tokenInfo[0][i].first;
							i++;
						}
						cout << eval.infixToPostfix(valueinput);
					}
				}
				else if (lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::IDENTIFIER)
				{
					if (lex.tokenInfo[0][i + 1].second == Lexical_Analyzer::categoryType::COMMA || lex.tokenInfo[0][i + 1].second == Lexical_Analyzer::categoryType::RIGHT_PAREN)
						cout << eval.getValue(lex.tokenInfo[0][i].first);
					else
					{
						string valueinput = "";
						while (lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::IDENTIFIER || lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::ASSIGNMENT_OP
							|| lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::RELATIONAL_OP || lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::NUMERIC_LITERAL
							|| lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::LOGICAL_OP || lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::ARITH_OP)
						{
							valueinput += lex.tokenInfo[0][i].first;
							i++;
						}
						cout << eval.infixToPostfix(valueinput);
					}

				}
				else if (lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::COMMA)
				{
					cout << " ";
				}
				else if (lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::UNKNOWN)
				{
					cout << "Error unrecognized syntax. Please review your code." << endl;
					exit(1);
				}

			}
			cout << endl;
		}
	}

	void showVariables()
	{
		eval.showVariables(); //calls function from evaluator class
	}

	void clear()
	{
		eval.clear();
		whileLoop.clear();
		ifStatement.clear();
	}

	bool if_loop = false, while_loop = false, if_executed = false, failedCondition = false, newIndent = false;
	int loop_index = 0;
	string whileCondition, loop_indent;

private:
	vector<string> whileLoop;
	vector<string> ifStatement;
	vector<vector<string>> elifBlock;
	vector<string> elseBlock;
};


#endif
