#include "ModbusDevice.h"
#include "../utils.h"
#include <spdlog/spdlog.h>

int ModbusDevice::numDevices = 0;
modbus_t *ModbusDevice::mb = nullptr;

ModbusDevice::ModbusDevice(const std::string& params, DeviceJson& conf) : DaqDevice(params, conf) {
	if (++numDevices == 1) {
		mb = modbus_new_rtu(params.c_str(), 
			115200, 'N', 8, 1);
		modbus_set_slave(mb, 15);
		int ret = modbus_connect(mb);
		if (ret != 0) {
			nullErr("Couldn't open device", 0, true);
		}
	}
}

uint16_t modbusRegs[32];
uint8_t modbusBits[32];
void ModbusDevice::poll(const std::string& id, std::vector<DATA_TYPE>& result) {
	if (mb == nullptr) {
		result.resize(1);
		result[0] = -420;
		spdlog::warn("Modbus was never intialized!");
		return;
	}
	int deviceId, reg, numReg;
	char regType;
	sscanf(id.c_str(), "%d+%c@%d:%d", &deviceId, &regType, &reg, &numReg);
	spdlog::debug("Parsed params: {} {} {} {} {}", deviceId, regType, reg, numReg);
	//modbus_set_slave(mb, deviceId);
	int ret;
	switch (regType) {
		case 'i': // input register
			ret = modbus_read_input_registers(mb, reg, numReg, modbusRegs);
			break;
		case 'c': // coils (1 bit read/write)
			ret = modbus_read_bits(mb, reg, numReg, modbusBits);
			break;
		case 'b': // bits (read)
			ret = modbus_read_input_bits(mb, reg, numReg, modbusBits);
			break;
		case 'h': // holding register (read/write)
			ret = modbus_read_registers(mb, reg, numReg, modbusRegs);
			break;
		default:
			result.resize(1);
			result[0] = -66;
			return;
	}
	if (ret != 0) {
		result.resize(1);
		result[0] = errno;
		spdlog::warn("Unable to read params {}: {} ({})", id, errno, modbus_strerror(errno));
		return;
	}
	result.resize(numReg);
	// copy registers
	if (regType == 'b' || regType == 'c') {
		for (int i = 0; i < numReg; i++) {
			result[i] = modbusBits[i];
		}
	}
	else {
		for (int i = 0; i < numReg; i++) {
			result[i] = modbusRegs[i];
		}
	}
	
}

ModbusDevice::~ModbusDevice() {
	if (--numDevices == 0) {
		modbus_close(mb);
		modbus_free(mb);
	}
}