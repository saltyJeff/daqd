//
// Created by Jefferson on 1/27/2020.
//

#ifndef DAQ_DAEMON_DAQSTATE_H
#define DAQ_DAEMON_DAQSTATE_H

#include "dataclasses/DaqConfigJson.h"
#include "DaqDevice.h"
#include "DaqItem.h"
#include <iostream>
#include <fstream>
#include <unordered_map>

class DaqState {
public:
	DaqConfigJson config;
	std::unordered_map<std::string, DaqDevice*> devices;
	std::unordered_map<std::string, std::vector<DaqItem>> items;
	DaqState(std::string backingPath);
	bool isLogging();
	void setLogging(bool logging);
	void logItem(DaqItem &item);
	void logItems(std::vector<DaqItem> &items);
	void loadConfig(DaqConfigJson &conf);
private:
	bool logging = false;
	std::ofstream logger;
	std::fstream backingFile;
	std::string backingPath;
};


#endif //DAQ_DAEMON_DAQSTATE_H
