#ifndef MODBUS_DEVICE_H
#define MODBUS_DEVICE_H

#include "../DaqDevice.h"
#include "../dataclasses/DeviceJson.h"
#include <modbus/modbus.h>

class ModbusDevice : public DaqDevice {
public:
	ModbusDevice(const std::string &params, DeviceJson& conf);
	virtual void poll(const std::string& id, std::vector<DATA_TYPE>& result);
	virtual ~ModbusDevice();
private:
	static bool connected;
	static modbus_t* mb;
};
#endif