#ifndef DAQD_RPC_H
#define DAQD_RPC_H

#include <cereal/archives/json.hpp>

class ReadConfig {
public:
	bool readConfig;

	template<class Archive>
	void serialize(Archive & arc) {
		arc(CEREAL_NVP(readConfig));
	}
};
const int8_t REC_ON = 1;
const int8_t REC_OFF = 0;
const int8_t REC_QUERY = -1;

class Recording {
public:
	int8_t recording;
	template<class Archive>
	void serialize(Archive& arc) {
		arc(CEREAL_NVP(recording));
	}
};
class ReadItems {
public:
	bool read;

	template<class Archive>
	void serialize(Archive& arc) {
		arc(CEREAL_NVP(read));
	}
};

#endif