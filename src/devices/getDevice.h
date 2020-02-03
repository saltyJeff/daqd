#ifndef DAQD_GETDEVICE
#define DAQD_GETDEVICE

#include "../DaqDevice.h"
#include "SineDevice.h"
#include "ModbusDevice.h"
#include "../dataclasses/DeviceJson.h"
#include <string>
inline DaqDevice* getDevice(const std::string name, DeviceJson& conf) {
	if (name == "sine") {
		return new SineDevice(conf.params, conf);
	}
	else if (name == "modbus") {
		return new ModbusDevice(conf.params, conf);
	}
	else {
		return nullptr;
	}
}

#endif