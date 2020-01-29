#include "utils.h"
#include <spdlog/spdlog.h>
#include <ctime>

time_t rawtime;
struct tm * timeinfo;
char buffer[80];
std::string timeString() {
	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer),"%m-%d-%Y--%H-%M-%S",timeinfo);
	std::string str(buffer);
	return str;
}

struct timespec timeSpec;
uint64_t millis() {
	clock_gettime(CLOCK_MONOTONIC_RAW, &timeSpec);
	return (timeSpec.tv_sec * 1000) + timeSpec.tv_nsec / (1000 * 1000);
}

void nullErr(std::string msg, void* anyPtr, bool fatal) {
	if (anyPtr != nullptr) {
		return;
	}
	std::string errMsg = msg + ": " + std::strerror(errno);
	spdlog::error(errMsg);
	if (fatal) {
		std::exit(1);
	}
	throw std::runtime_error(errMsg);
}