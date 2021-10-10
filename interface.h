#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class Interface
{
public:
	
	void startInterface() 
	{
		// Displays initial menu text
		cout << "PySUB Interpreter 1.0 on Windows (September 2021)" << endl
			 << "Enter program lines or read(<filename>.py) at command line interface" << endl
			 << "Type 'help' for more information or 'quit' to exit" << endl;

		string input;
		do //interface will loop the input until quit is received
		{
			cout << ">>> ";
			cin >> input;

			//options for input
			if (input[0] == 'h' && input[1] == 'e' && input[2] == 'l' && input[3] == 'p')
			{
				if (input == "help" || input == "help()") //run help command
					help();
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
			else if (input == "clear" || input == "clear()") //runs clear
				clear();
			else if (input == "quit" || input == "quit()") //runs quit
				quit();
			else
				cout << "Error unrecognized command." << endl;

			cout << endl;
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
				cout << "This command deletes any previous .py file that has been imported into the interface." << endl;
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
		}
		file.close(); //close file when done
	}

	void show() {
		for (int i = 0; i < programCode.size(); i++) //displays all lines stored in the vector
		{
			cout << "[" << i << "] " << programCode[i] << endl;
		}
	}

	void clear() {
		programCode.clear(); //deletes all lines from the vector
	}

private:
	typedef vector<string> programType;
	programType programCode; //vector<string> holding all the lines from python files
};



#endif