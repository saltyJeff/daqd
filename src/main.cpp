#include "DaqState.h"
#include "poll.h"
#include <iostream>
#include "SocketHandler.h"

int main() {
	DaqState conf("/home/jefferson/.daqd.json");
	SocketHandler server(&conf);
	server.openSocket();

	while (true) {
		poll(conf);
		server.handleSocket();
	}
}