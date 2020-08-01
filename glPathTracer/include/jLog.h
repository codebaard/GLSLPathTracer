#ifndef JLOG_H
#define JLOG_H

/* jLog Logger class
Written by Julius Neudecker
v0.1 - 24.07.2020
v0.2 - 27.07.2020 -> Added glGetError-Handler
v0.3 - 30.07.2020 -> Added overloaded Log()-Function for Strings
*/

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <time.h>

#define LOG_FILENAME "jlogger.log"

/* ToDo:
- finish timestamp
- Include Stacktrace for calling function name
- File: new, overwrite, append
- Set File Name + Location
*/


enum TimestampOptions {
	DATEONLY,
	DATETIME24H,
	DATETIME12H,
	DATETIME24HWITHSECONDS,
	DATETIME12HWITHSECONDS,
	NOTIMESTAMP
};

enum LogLevel {
	ERRORONLY,
	WARNINGS,
	INFOS,
	DEBUGGING
};

enum LogOutput {
	CONSOLE,
	TOFILE
};

enum MessageType {
	ERROR,
	WARNING,
	INFO,
	DEBUG
};

class jLog {
public:
	static jLog* Instance() {
		static CGuard g; //Memory management
		if (!_instance) {
			_instance = new jLog();
		}
		return _instance;
	}

	void ConfigLogger(TimestampOptions, LogLevel, LogOutput);
	void Error(const char*);
	void Error(std::string);
	void Log(MessageType, const char*);
	void Log(MessageType, std::string);
	void glError(unsigned int errCode, std::string functionCall);
	void glError(std::string err);
	void glCallbackError(std::string errMessage, unsigned int errID);

private:
	static jLog* _instance;
	jLog();
	jLog(const jLog&);
	~jLog();

	unsigned int _logLevel;
	unsigned int _logOutput;
	unsigned int _timestampOptions;
	std::ofstream _logFile;
	//time_t _systemTime;

	std::string _timestamp();
	void _createFile(const char*);
	void _writeLog(const char*);

	class CGuard
	{
	public:
		~CGuard()
		{
			if (NULL != jLog::_instance)
			{
				delete jLog::_instance;
				jLog::_instance = NULL;
			}
		}
	};
};


#endif