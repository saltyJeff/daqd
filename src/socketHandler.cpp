#include "socketHandler.h"
#include <sockpp/unix_acceptor.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <chrono>
#include "utils.h"
#include "DaqState.h"
#include "rpcs.h"
#include <cereal/inlineshim.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <cstring>
#include <unistd.h>

const char* SOCK_PATH = "/tmp/daqd.sock";

void SocketHandler::openSocket() {
	spdlog::info("Opening socket at /tmp/daqd.sock");
	unlink(SOCK_PATH);
	bool ok = acc.open(sockpp::unix_address(SOCK_PATH));
	if (!ok) {
		spdlog::error("Could not open socket! {}", acc.last_error_str());
	}
	acc.set_non_blocking();
}

const int buffSize = 1024;
char buff[buffSize];

void SocketHandler::handleSocket() {
	auto now = millis();
	auto sock = acc.accept();
	if (!sock) {
		return;
	}
	spdlog::info("Client connected");
	sock.read_timeout(std::chrono::seconds(1));
	int i = 0;
	int n;
	while ( (n = sock.read(buff + i, buffSize)) > 0) {
		for (int j = i; i < i + n; j++) {
			if (buff[j] == '\n') {
				goto READ_DONE;
			}
		}
		i += n;
	}
READ_DONE:	
	if (n < 0) {
		spdlog::error("Error reading from client: {}", sock.last_error_str());
		return;
	}
	if (n == 0) {
		spdlog::error("Timeout on read");
		return;
	}
	std::string inputStr = std::string(buff, n);

	sock.write(processCommand(inputStr));
	sock.close();
	auto delta = millis() - now;
	spdlog::info("Serviced client in {} ms", delta);
}
// unfortuantely c++ is not going to give me a nice way to put these into a list
std::string SocketHandler::processCommand(std::string& cmd) {
	std::stringstream inputStream(cmd);
	std::string response;
	
	response = handleNewConfig(inputStream);
	if (response.length() != 0) {
		return response;
	}
	resetSS(inputStream, response);
	response = handleReadConfig(inputStream);
	if (response.length() != 0) {
		return response;
	}
	resetSS(inputStream, response);
	response = handleReadItems(inputStream);
	if (response.length() != 0) {
		return response;
	}
	resetSS(inputStream, response);
	response = handleRecording(inputStream);
	if (response.length() != 0) {
		return response;
	}
	return errJson("Unable to process command");
}
std::string SocketHandler::handleReadConfig(std::istream& inputStream) {
	ReadConfig readConf;
	try {
		cereal::JSONInputArchive ar(inputStream);
		cereal::load_inline(ar, readConf);
	}
	catch (std::exception& e) {
		int n = strcspn(e.what(), "RapidJSON");
		if (n == 0) {
			return "";
		}
		return errJson(e.what());
	}
	std::stringstream ostream;
	{
		cereal::JSONOutputArchive ar(ostream);
		cereal::save_inline(ar, state->config);
	}
	return ostream.str();
}
std::string SocketHandler::handleNewConfig(std::istream& inputStream) {
	DaqConfigJson newConf;
	try {
		cereal::JSONInputArchive ar(inputStream);
		cereal::load_inline(ar, newConf);
	}
	catch (std::exception& e) {
		int n = strcspn(e.what(), "RapidJSON");
		if (n == 0) {
			return "";
		}
		return errJson(e.what());
	}
	try {
		state->loadConfig(newConf);
	}
	catch (std::exception& e) {
		return errJson(e.what());
	}
	std::stringstream ostream;
	{
		cereal::JSONOutputArchive ar(ostream);
		cereal::save_inline(ar, state->config);
	}
	return ostream.str();
}
std::string SocketHandler::handleReadItems(std::istream& inputStream) {
	ReadItems readItems;
	try {
		cereal::JSONInputArchive ar(inputStream);
		cereal::load_inline(ar, readItems);
	}
	catch (std::exception& e) {
		int n = strcspn(e.what(), "RapidJSON");
		if (n == 0) {
			return "";
		}
		return errJson(e.what());
	}
	std::stringstream ostream;
	{
		cereal::JSONOutputArchive ar(ostream);
		cereal::save_inline(ar, state->items);
	}
	return ostream.str();
}
std::string SocketHandler::handleRecording(std::istream& inputStream) {
	Recording recording;
	try {
		cereal::JSONInputArchive ar(inputStream);
		cereal::load_inline(ar, recording);
	}
	catch (std::exception& e) {
		int n = strcspn(e.what(), "RapidJSON");
		if (n == 0) {
			return "";
		}
		return errJson(e.what());
	}
	try {
		if (recording.recording == REC_QUERY) {
			recording.recording = state->isLogging();
		}
		else {
			state->setLogging(!!recording.recording);
		}
	}
	catch (std::runtime_error &e) {
		return errJson(e.what());
	}
	std::stringstream ostream;
	{
		cereal::JSONOutputArchive ar(ostream);
		cereal::save_inline(ar, recording);
	}
	return ostream.str();
}