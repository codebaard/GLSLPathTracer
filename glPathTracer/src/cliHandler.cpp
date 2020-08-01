#include <cliHandler.h>

cliHandler::cliHandler(int argc, char* argv[])
{
	const char* tmp;
	std::list<std::string> params;

	for (int i = 0; i < argc; i++) {
		tmp = argv[i];
		params.push_back(std::string(tmp));
	}

	std::list<std::string>::iterator it = params.begin();

	CWD = *it++;

	if (!_validateCLI(*it++)) {
		throw (std::exception("Please use the launcher app provided to start the renderer."));
	}

	ScreenWidth = stoi(*it++);
	ScreenHeight = stoi(*it++);
	FilePathToModel = *it++;
	ModelName = *it; //last element. Avoid pointing to undefined
	
	CWD = CWD.substr(0, CWD.find_last_of('\\'));
}

bool cliHandler::_validateCLI(std::string key) {

	if (key == _key) {
		return true;;
	}
	return false;
}

cliHandler::~cliHandler()
{
}