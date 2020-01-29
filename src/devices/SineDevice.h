//
// Created by Jefferson on 1/27/2020.
//

#ifndef DAQ_DAEMON_SINEDEVICE_H
#define DAQ_DAEMON_SINEDEVICE_H
#include "../DaqDevice.h"

class SineDevice: public DaqDevice {
public:
	SineDevice(std::string devParam, DeviceJson &conf): DaqDevice(devParam, conf) {};
	std::vector<DATA_TYPE> poll(std::string id);
};


#endif //DAQ_DAEMON_SINEDEVICE_H
