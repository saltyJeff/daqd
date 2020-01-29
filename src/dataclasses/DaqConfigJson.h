//
// Created by Jefferson on 1/27/2020.
//

#ifndef DAQ_DAEMON_DAQCONFIGJSON_H
#define DAQ_DAEMON_DAQCONFIGJSON_H

#include <string>
#include <unordered_map>
#include "ItemJson.h"
#include "DeviceJson.h"
#include <cereal/archives/json.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <vector>

class DaqConfigJson {
public:
	std::string logPath = "ROTATE";
	std::unordered_map<std::string, DeviceJson> devices;
	std::unordered_map<std::string, std::vector<ItemJson>> items;

	template<class Archive>
	void serialize(Archive & archive) {
		archive(CEREAL_NVP(logPath), CEREAL_NVP(devices), CEREAL_NVP(items));
	}
};
#endif //DAQ_DAEMON_DAQCONFIGJSON_H
