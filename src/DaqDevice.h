//
// Created by Jefferson on 1/27/2020.
//

#ifndef DAQ_DAEMON_DEVICE_H
#define DAQ_DAEMON_DEVICE_H

#include <string>
#include <cstdint>
#include <vector>
#include "dataclasses/PollMode.h"
#include "COMPILE_OPTS.h"
#include "dataclasses/DeviceJson.h"

class DaqDevice {
public:
	PollMode pollMode = PollMode::ROUND_ROBIN;
	uint32_t pollRate = 100;
	uint64_t lastPoll = 0;
	uint8_t pollIndex = 0;
	DaqDevice(std::string devParam, DeviceJson& conf): pollMode(static_cast<PollMode>(conf.pollMode)), pollRate(conf.pollRate) {}
	virtual void poll(const std::string& id, std::vector<DATA_TYPE>& result) = 0;
	virtual ~DaqDevice() {};
};
#endif //DAQ_DAEMON_DEVICEJSON_H
