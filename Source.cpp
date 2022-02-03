#include <iostream>
#include "interface.h"
#include "lexanalyzer.h"
#include "expevaluator.h"

using namespace std;

int main()
{
	Interface pySubInterpreter;
	//Start the interface
	pySubInterpreter.startInterface();

	return 0;
}