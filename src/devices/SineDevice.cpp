#include "SineDevice.h"
#include <math.h>
#include "../utils.h"

std::vector<DATA_TYPE> SineDevice::poll(std::string id) {
	return std::vector<DATA_TYPE> { (DATA_TYPE) (8.0*sin(millis()+0.0)) };
}