#include "DaqState.h"
#include "devices/SineDevice.h"
#include "utils.h"
#include <spdlog/spdlog.h>
#include <cereal/inlineshim.hpp>
#include <cereal/archives/json.hpp>
#include <regex>

DaqState::DaqState(const std::string backingPath): backingPath(backingPath) {
	spdlog::info("Loading config file from " + backingPath);
	// ensure the existance of the file
	backingFile.open(backingPath, std::fstream::in);
	DaqConfigJson tmp;
	tmp.items["potato"].push_back(ItemJson());
	try {
		cereal::JSONInputArchive iarchive(backingFile);
		cereal::load_inline(iarchive, tmp);
		spdlog::info("Loaded config from"+backingPath);
	}
	catch (std::exception& err) {
		spdlog::error("Could not open config: {}", err.what());
	}
	backingFile.close();
	loadConfig(tmp);
}

void DaqState::loadConfig(DaqConfigJson &conf) {
	bool prevClosing = isLogging();
	setLogging(false);
	this->config = conf;
	// close all existing devices
	for(auto& [devName, dev] : devices) {
		delete dev;
	}
	// open new devices
	devices.clear();
	for(auto& [devName, devConf] : config.devices) {
		// spawn the correct type of device
		DaqDevice *dev = nullptr;
		if(devConf.devType == "sine") {
			dev = new SineDevice(devConf.params, devConf);
		}
		else {
			throw new std::runtime_error(std::string("Invalid device type specified"));
		}
		spdlog::info("Created a {} device", devConf.devType);
		devices[devName] = dev;
	}
	// copy over new items;
	items.clear();
	for(auto& [devName, itemConfs] : config.items) {
		std::vector<DaqItem> itemList;
		for(auto itemConf : itemConfs) {
			itemList.push_back(DaqItem(itemConf));
		}
		items[devName] = itemList;
	}
	//write backing state
	backingFile.open(backingPath, std::fstream::trunc | std::fstream::out);
	{
		cereal::JSONOutputArchive oarchive(backingFile);
		cereal::save_inline(oarchive, config);
	}
	backingFile.close();
	spdlog::info("Saved config");

	// restore previous state
	setLogging(prevClosing);
	spdlog::info("Loaded new configuration");
}
void DaqState::setLogging(bool logging) {
	if(logging == this->logging) {
		return;
	}
	if(logging) {
		std::string logPath = std::regex_replace(config.logPath, std::regex("ROTATE"), timeString()); //handle ROTATE special string
		logger.open(logPath, std::ios_base::out | std::ios_base::app);
		if (logger.fail()) {
			nullErr("Could not open log file", 0);
		}
		spdlog::info("Opened log file: "+logPath);
	}
	else {
		logger.flush();
		logger.close();
	}
	this->logging = logging;
	spdlog::info("Logging state: {}", logging);
}
bool DaqState::isLogging() {
	return logging;
}
void DaqState::logItem(DaqItem &item) {
	if(!logging) {
		return;
	}
	item.writeToStream(logger);
	logger.flush();
	if (logger.fail()) {
		spdlog::error("Could not successfully write to the log file. Stopping the log");
		setLogging(false);
	}
}
void DaqState::logItems(std::vector<DaqItem> &items) {
	if(!logging) {
		return;
	}
	for(auto item : items) {
		item.writeToStream(logger);
	}
	logger.flush();
	if (logger.fail()) {
		spdlog::error("Could not successfully write to the log file. Stopping the log");
		setLogging(false);
	}
}