//
// Created by Jefferson on 1/27/2020.
//

#ifndef DAQ_DAEMON_ITEMJSON_H
#define DAQ_DAEMON_ITEMJSON_H

#include <cstdint>
#include <cereal/archives/json.hpp>
#include "../COMPILE_OPTS.h"
#include <string>

class ItemJson {
public:
	std::string name;
	std::string id;
	DATA_TYPE offset;
	DATA_TYPE scale;

	template<class Archive>
	void serialize(Archive &archive) {
		archive(CEREAL_NVP(name), CEREAL_NVP(id), CEREAL_NVP(offset), CEREAL_NVP(scale));
	};
};

#endif //DAQ_DAEMON_ITEMJSON_H
