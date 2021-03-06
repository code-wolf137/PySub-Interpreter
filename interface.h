#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "lexanalyzer.h"
#include "expevaluator.h"
#include "interpreter.h"

using namespace std;

class Interface
{
public:

	friend class Interpreter;

	Lexical_Analyzer lex;

	expEvaluator eval;

	Interpreter pysubi;

	void startInterface()
	{
		// Displays initial menu text
		cout << "PySUB Interpreter 5.0 on Windows (December 2021)" << endl
			<< "Enter program lines or read(<filename>.py) at command line interface" << endl
			<< "Type 'help' for more information or 'quit' to exit" << endl;

		string input;
		do //interface will loop the input until quit is received
		{
			if (pysubi.while_loop || pysubi.if_loop || pysubi.failedCondition)
				cout << "... ";
			else
				cout << ">>> ";
			getline(cin, input);

			if (input[0] == 'h' && input[1] == 'e' && input[2] == 'l' && input[3] == 'p')
			{
				if (input == "help" || input == "help()") {//run help command
					help();
				}
				else //retrieves the name of the command from help(<command>) input
				{
					string command = "";
					for (int i = 5; i < input.length(); i++)
					{
						if (input[i] == ')')
							break;
						else
							command = command + input[i];

					}
					help(command); //runs the help command using a string with the command name

				}
			}
			else if (input[0] == 'r' && input[1] == 'e' && input[2] == 'a' && input[3] == 'd')
			{
				string filename = "";
				for (int i = 5; i < input.length(); i++) //retrives file name
				{
					if (input[i] == '.')
					{
						filename = filename + ".py";
						break;
					}
					else
					{
						filename = filename + input[i];
					}
				}
				read(filename); //reads filename
			}
			else if (input == "show" || input == "show()") //runs show
				show();
			else if (input == "run" || input == "run()") //runs show
				run();
			else if (input[0] == 's' && input[1] == 'h' && input[2] == 'o' && input[3] == 'w')
			{
				string tokens = "";
				for (int i = 5; i < input.length(); i++) //retrives file name
				{
					if (input[i] == ')')
					{
						break;
					}
					else
					{
						tokens = tokens + input[i];
					}
				}
				show(tokens); //reads filename
			}
			else if (input == "clear" || input == "clear()") //runs clear
				clear();
			else if (input == "quit" || input == "quit()") //runs quit
				quit();
			else
			{
				bool Continue = false;
				//options for input
				lex.startLexAnalysis(input, 0);
				for (int i = 0; i < lex.tokenInfo[0].size(); i++)
				{
					if ((lex.tokenInfo[0][i].second == Lexical_Analyzer::categoryType::UNKNOWN && !(lex.tokenInfo[0][i].first == "." && lex.tokenInfo[0][i + 1].first == "py")) || (lex.tokenInfo[0].size() == 1 && lex.tokenInfo[0][0].second == Lexical_Analyzer::categoryType::IDENTIFIER)
						|| (lex.tokenInfo[0].size() == 2 && lex.tokenInfo[0][1].second == Lexical_Analyzer::categoryType::IDENTIFIER && lex.tokenInfo[0][0].second == Lexical_Analyzer::categoryType::INDENT))
					{
						cout << "Syntax error please revise" << endl << endl;
						Continue = true;
						lex.tokenInfo.clear();
						break;
					}
				}
				if (Continue)
					continue;
				pysubi.interpretCode(input);
			}
			

		} while (input != "quit" && input != "quit()"); //menu loop runs until quit is entered
	}


	void quit() {
		exit(0);
	}


	void help() {
		//help menu displays
		cout << "Welcome to the help utility!" << endl
			<< "* To exit and return to the interpreter, type 'exit'" << endl
			<< "* To get a list of commands, type 'commands'" << endl
			<< "* To get a description of any command, just type the command at the 'help>' prompt" << endl << endl;

		string helpInput;
		do {
			cout << "help> ";
			cin >> helpInput;

			//options based on input string helpInput
			if (helpInput == "exit")
				return;
			else if (helpInput == "commands")
			{
				cout << "Below is a list of commands. Enter any command at the prompt to get more help." << endl << endl;
				cout << "clear\thelp\tquit" << endl << "read\tshow" << endl << endl;

				cout << "NOTE:  All commands can also be entered as functions:" << endl << endl;
				cout << "clear()\thelp()\tquit()" << endl << "read()\tshow()" << endl << endl;
			}
			//describes all the commands
			else if (helpInput == "clear" || helpInput == "clear()")
			{
				cout << "This command deletes any previous .py file as well as the \ncorresponding variables and tokens that has been imported into the interface." << endl;
			}
			else if (helpInput == "help" || helpInput == "help()")
			{
				cout << "This command gives more information about interface commands." << endl;
			}
			else if (helpInput == "quit" || helpInput == "quit()")
			{
				cout << "This command exits the interpreter interface." << endl;
			}
			else if (helpInput == "read" || helpInput == "read()")
			{
				cout << "This command is able to read a .py file and import it into the interface." << endl;
			}
			else if (helpInput == "show" || helpInput == "show()")
			{
				cout << "This command displays the .py file that has already been imported into the interface\nthrough the read() command" << endl;
			}
			else if (helpInput == "exit" || helpInput == "exit()")
			{
				return;
			}
			else
			{
				cout << "Error unrecognized command." << endl;
				return;
			}
			cout << "To exit the help utility and return to the interpreter, enter 'exit' or you can enter more commands" << endl << endl;

		} while (helpInput != "exit");
	}


	void help(string command) {
		if (command == "clear" || command == "clear()") //describes all the commands
		{
			cout << "This command deletes any previous .py file that has been imported into the interface." << endl;
		}
		else if (command == "help" || command == "help()")
		{
			cout << "This command gives more information about interface commands." << endl;
		}
		else if (command == "quit" || command == "quit()")
		{
			cout << "This command exits the interpreter interface." << endl;
		}
		else if (command == "read" || command == "read()")
		{
			cout << "This command is able to read a .py file and import it into the interface." << endl;
		}
		else if (command == "show" || command == "show()")
		{
			cout << "This command displays the .py file that has already been imported into the interface\nthrough the read() command" << endl;
		}
		else
		{
			cout << "Error unrecognized command." << endl;
			return;
		}

		string exitInput; //loop to exit
		do
		{
			cout << "To exit the help utility and return to the interpreter, enter 'exit'" << endl;
			cout << ">>> ";
			cin >> exitInput;
			if (exitInput == "exit")
				return;
			else
			{
				cout << "Error please try again." << endl << endl;
			}
		} while (true);

	}


	void read(string filename) {
		programCode.clear(); //clears any data in the data structure
		lex.tokenInfo.clear();
		int lineCount = 0;
		string fileLine;
		ifstream file;
		file.open(filename); //opens file
		if (!file)
		{
			cout << "Error opening file." << endl;
			return;
		}
		while (!file.eof()) //while the end of file has not been reached
		{
			getline(file, fileLine); //retrives entire line
			programCode.push_back(fileLine); //stores in vector
			lex.startLexAnalysis(fileLine, lineCount); //sends line to startlexanalysis
			lineCount++;
		}
		file.close(); //close file when done

	}


	void show() {
		cout << endl;
		for (int i = 0; i < programCode.size(); i++) //displays all lines stored in the vector
		{
			cout << "[" << i << "] " << programCode[i] << endl;
		}
		cout << endl;
	}


	void clear() {
		pysubi.clear();
		programCode.clear(); //deletes all lines from the vector
		lex.clear(); //calls function from lexanalyzer class
		eval.clear();
	}


	void show(string input)
	{
		if (input == "tokens")
			lex.show(); //calls function from lexanalyzer class
		else if (input == "variables") {
			cout << endl;
			pysubi.showVariables(); //calls function from interpreter class
		}
		else
		{
			cout << "Error unrecognized command." << endl;
			return;
		}

	}


	vector<string> importData() //creates a vector that can transfer data from programCode to lex analyzer class
	{
		vector<string> data;

		for (int i = 0; i < programCode.size(); i++)
		{
			data.push_back(programCode[i]);
		}

		return data;
	}

	bool is_expression(string input)
	{
		Lexical_Analyzer expressionLex;
		return expressionLex.is_expression(input);
	}

	void run()
	{
		pysubi.run(programCode);
		cout << endl;
	}


private:

	typedef vector<string> programType;
	programType programCode; //vector<string> holding all the lines from python files

};

#endif
