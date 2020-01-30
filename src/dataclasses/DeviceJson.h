#ifndef DAQ_DAEMON_DEVICEJSON_H
#define DAQ_DAEMON_DEVICEJSON_H

#include <string>
#include <cstdint>
#include <cereal/archives/json.hpp>
#include "PollMode.h"

class DeviceJson {
public:
	uint8_t pollMode;
	std::string params;
	std::string devType;
	uint32_t pollRate;

	template<class Archive>
	void serialize(Archive &archive) {
		archive(CEREAL_NVP(pollMode), CEREAL_NVP(params), CEREAL_NVP(devType), CEREAL_NVP(pollRate));
	}
};
#endif //DAQ_DAEMON_DEVICEJSON_H
