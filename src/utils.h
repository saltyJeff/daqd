#ifndef DAQ_DAEMON_UTILS_H
#define DAQ_DAEMON_UTILS_H
#include <string>
#include <ctime>

std::string timeString();
uint64_t millis();
void nullErr(std::string msg, void *anyPtr, bool fatal=false);

#endif //DAQ_DAEMON_UTILS_H
