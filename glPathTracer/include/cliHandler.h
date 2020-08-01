/*
This class handles all the command line input and distributes the parmetres set.
Since this application is started by a launcher application, theres no necessity to
validate parametres.

written by Julius Neudecker
v0.1 - 01.08.2020
*/
#ifndef CLIHANDLER_H
#define CLIHANDLER_H

#include <list>
#include <string>

class cliHandler
{
public:
	cliHandler(int argc, char* argv[]);
	~cliHandler();

	int ScreenWidth;
	int ScreenHeight;

	std::string CWD; //current workling directory
	std::string FilePathToModel;
	std::string ModelName;

private:
	bool _validateCLI(std::string key);

	//some arbitrary key to prevent launching from the cli directly.
	std::string _key = "6OWNObpW4iBrqNnA8r2L";
};


#endif
