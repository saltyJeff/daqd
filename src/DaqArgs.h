#ifndef ARG_PARSE_H
#define ARG_PARSE_H

#include <string>

class DaqArgs {
public:
	std::string configPath = "~/.daqd.json";
	bool autoRecord = false;
	bool realtime = false;
	std::string username;

	DaqArgs(int argc, char** argv);
};

#endif