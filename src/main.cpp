#include "DaqState.h"
#include "poll.h"
#include <iostream>
#include "SocketHandler.h"
#include <csignal>
#include <pthread.h>
#include <sched.h>
#include <spdlog/spdlog.h>
#include <ctime>
#include "utils.h"
#include <unistd.h>
#include <pwd.h>
#include "DaqArgs.h"

void signalHandle(int i);
void setRealtime();
bool die = false;
clockid_t sleepType = CLOCK_MONOTONIC;
struct timespec sleepSpec;

int main(int argc, char **argv) {
	DaqArgs daqArgs(argc, argv);
	if (daqArgs.realtime) {
		setRealtime();
	}
	sleepSpec.tv_sec = 1;
	signal(SIGINT, signalHandle);
	signal(SIGTERM, signalHandle);
	
	DaqState conf(daqArgs.configPath);
	SocketHandler server(&conf);
	server.openSocket();

	conf.setLogging(daqArgs.autoRecord);

	while (!die) {
		if (conf.isLogging()) {
			poll(conf);
		}
		server.handleSocket();
		if (conf.config.granularityMicro == 0) {
			continue;
		}
		microsToTimespec(conf.config.granularityMicro, &sleepSpec);
		int sleepErr = clock_nanosleep(sleepType, 0, &sleepSpec, nullptr);
		if (sleepErr != 0) {
			spdlog::error("Sleep issue: {} ({})\nSwitching clock type to REALTIME (are you on WSL?)", sleepErr, std::strerror(sleepErr));
			std::cout << sleepSpec.tv_sec << ": " << sleepSpec.tv_nsec << std::endl;
			sleepType = CLOCK_REALTIME;
		}
	}
	spdlog::warn("Goodbye!");
}

void signalHandle(int i) {
	spdlog::warn("caught sigint");
	die = true;
}

void setRealtime() {
	pthread_t this_thread = pthread_self();
	struct sched_param params;
	// Set priority to round - robin
	params.sched_priority = sched_get_priority_max(SCHED_RR);
	int ret = pthread_setschedparam(this_thread, SCHED_FIFO, &params);
	if (ret != 0) {
		spdlog::error("Could not set the thread's scheduling into realtime mode: {} ({})", ret, std::strerror(ret));
	}
}