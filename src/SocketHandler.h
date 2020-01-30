#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H

#include "DaqState.h"
#include <string>
#include <sockpp/unix_acceptor.h>

class SocketHandler {
public:
	DaqState *state;
	SocketHandler(DaqState *state) : state(state) {};
	void openSocket();
	void handleSocket();

private:
	sockpp::socket_initializer sockInit;
	sockpp::unix_acceptor acc;
	std::string processCommand(std::string& cmd);
	std::string handleReadConfig(std::istream& inputStream);
	std::string handleNewConfig(std::istream& inputStream);
	std::string handleReadItems(std::istream& inputStream);
	std::string handleRecording(std::istream& inputStream);
};

inline std::string errJson(std::string msg) {
	return "{\"err\": \"" + msg + "\"}";
}
inline void resetSS(std::stringstream& ss, std::string &str) {
	ss.clear();
	ss.seekg(0, std::ios::beg);
}

#endif