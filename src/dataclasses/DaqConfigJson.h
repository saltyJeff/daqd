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
	std::string logPath = "~/ROTATE.daqd.log";
	std::uint64_t granularityMicro = 1000;
	std::unordered_map<std::string, DeviceJson> devices;
	std::unordered_map<std::string, std::vector<ItemJson>> items;

	template<class Archive>
	void serialize(Archive & archive) {
		archive(CEREAL_NVP(logPath), CEREAL_NVP(devices), CEREAL_NVP(items), CEREAL_NVP(granularityMicro));
	}
};
#endif //DAQ_DAEMON_DAQCONFIGJSON_H
