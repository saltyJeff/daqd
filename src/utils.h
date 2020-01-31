#ifndef DAQ_DAEMON_UTILS_H
#define DAQ_DAEMON_UTILS_H
#include <string>
#include <ctime>

std::string timeString();
uint64_t millis();
uint64_t realMillis();

void nullErr(std::string msg, void *anyPtr, bool fatal=false);

inline void microsToTimespec(uint64_t micros, struct timespec* spec) {
	uint64_t secs = micros / 1000 / 1000;
	uint64_t nanoSecs = ( micros % (1000 * 1000) ) * 1000;
	spec->tv_sec = secs;
	spec->tv_nsec = nanoSecs;
}

#endif //DAQ_DAEMON_UTILS_H
