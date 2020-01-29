#include "utils.h"
#include <chrono>
#include <spdlog/spdlog.h>

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

uint64_t millis() {
	using namespace std::chrono;
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
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