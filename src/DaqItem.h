//
// Created by Jefferson on 1/27/2020.
//

#ifndef DAQ_DAEMON_ITEM_H
#define DAQ_DAEMON_ITEM_H

#include <string>
#include <cstdint>
#include <vector>
#include "COMPILE_OPTS.h"
#include "dataclasses/ItemJson.h"
#include <iostream>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

class DaqItem {
public:
	std::string name;
	std::string id;
	DATA_TYPE offset;
	DATA_TYPE scale;
	uint64_t lastPoll = 0;
	std::vector<DATA_TYPE> values;
	DaqItem() {}; // exists for serialization purposes only
	DaqItem(ItemJson &conf): name(conf.name), id(conf.id), offset(conf.offset), scale(conf.scale){};
	void writeToStream(std::ostream &stream);
	void updateValues(const std::vector<DATA_TYPE> &newVals);

	template <class Archive>
	void serialize(Archive& archive) {
		archive(CEREAL_NVP(name), CEREAL_NVP(id), CEREAL_NVP(lastPoll), CEREAL_NVP(values));
	}
};

#endif //DAQ_DAEMON_ITEMJSON_H
