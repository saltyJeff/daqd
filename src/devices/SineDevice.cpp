#include "SineDevice.h"
#include <math.h>
#include "../utils.h"

void SineDevice::poll(const std::string &id, std::vector<DATA_TYPE> &result) {
	result = std::vector<DATA_TYPE> { (DATA_TYPE) (8.0*sin(millis()+0.0)) };
}