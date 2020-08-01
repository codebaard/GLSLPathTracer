/*
This class handles all the command line input and distributes the parmetres set.
Since this application is started by a launcher application, theres no necessity to
validate parametres.

written by Julius Neudecker
v0.1 - 01.08.2020
*/
#ifndef CLIHANDLER_H
#define CLIHANDLER_H

//some arbitrary key to prevent launching from the cli directly.
#define APPKEY "6OWNObpW4iBrqNnA8r2L"

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

	std::string _key = APPKEY;
};


#endif
