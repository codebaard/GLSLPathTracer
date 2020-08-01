#include <jLog.h>

jLog::jLog() {
	
	//set default config
	_logLevel = ERRORONLY;
	_logOutput = CONSOLE;
	_timestampOptions = DATETIME24H;
}

jLog::~jLog() {
	_logFile.close();
}

void jLog::ConfigLogger(TimestampOptions ts, LogLevel ll, LogOutput lo) {
	_logLevel = ll;
	_logOutput = lo;

	if (ll == DEBUG) {
		_timestampOptions = DATETIME24HWITHSECONDS; //Debug is always maximum timestamp info
	}
	else {
		_timestampOptions = ts;
	}


	if (lo == TOFILE) {
		_createFile(LOG_FILENAME);
	}

	Log(INFO, "jLog configured and ready.");
}

void jLog::_createFile(const char* filename) {
	_logFile.open(filename);
}

void jLog::Error(const char* msg) {
	std::string str = "";
	str = _timestamp() + " : ERROR : " + msg;
	_writeLog(str.c_str());
}

void jLog::Error(std::string msg) {
	Error(msg.c_str());
}

void jLog::Log(MessageType type, const char* msg) {
	if (type == ERROR) {
		Error(msg);
		return;
	}

	std::string str = "";

	switch (type) {
		case WARNING:
			if (_logLevel >= WARNINGS) {
				str = _timestamp() + " : WARNING : " + msg;
			}
			break;
		case INFO:
			if (_logLevel >= INFOS) {
				str = _timestamp() + " : INFO : " + msg;
			}
			break;
		default:
			if (_logLevel >= DEBUGGING) {
				str = _timestamp() + " : DEBUG : " + msg;
			}
	}

	_writeLog(str.c_str());
}

void jLog::Log(MessageType type, std::string msg) {
	Log(type, msg.c_str());
}

void jLog::glError(unsigned int errCode, std::string functionCall) {
	std::string str = "";
	str = _timestamp() + " : openGL Errorcode : " + std::to_string(errCode) + " in function: " + functionCall;

	_writeLog(str.c_str());
}

void jLog::glError(std::string err) {
	std::string str = "";
	str = _timestamp() + " : openGL Errormessage : " + err;

	_writeLog(str.c_str());
}

void jLog::glCallbackError(std::string errMessage, unsigned int errID) {
	std::string str = "";
	str = _timestamp() + " : openGL Errorcode : " + std::to_string(errID) + " : " + errMessage;

	_writeLog(str.c_str());
}

void jLog::_writeLog(const char* msg) {
	if (_logOutput == CONSOLE) {
		std::cout << msg << std::endl;
	}
	else {
		_logFile << msg << std::endl;
	}
}

std::string jLog::_timestamp() {
	struct tm newtime;
	time_t now = time(0);
	errno_t ltm = localtime_s(&newtime, &now);

	std::string ts = "";

	switch (_timestampOptions) {
	case NOTIMESTAMP:
		return ts;
	default:
		std::string year = std::to_string(1900 + newtime.tm_year);
		std::string month = std::to_string(1 + newtime.tm_mon);
		std::string day = std::to_string(newtime.tm_mday);
		std::string hour = std::to_string(1 + newtime.tm_hour);
		std::string min = std::to_string(1 + newtime.tm_min);
		std::string sec = std::to_string(1 + newtime.tm_sec);
		ts = year + month + day + " " + hour + min + sec;
	}

	return ts;
}

jLog* jLog::_instance = 0;