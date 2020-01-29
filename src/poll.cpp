#include "poll.h"
#include "utils.h"
#include <spdlog/spdlog.h>
#include <vector>
void poll(DaqState &state) {
	uint64_t now = millis();
	for(auto &[devName, dev] : state.devices) {
		if(now < dev->lastPoll + dev->pollRate) {
			continue;
		}
		int32_t delta = (now - (dev->lastPoll + dev->pollRate));
		if(delta > 10) {
			spdlog::warn("Missed target poll rate for {} by {} ms", devName, delta);
		}

		std::vector<DaqItem> &items = state.items[devName];
		if(dev->pollMode == PollMode::ROUND_ROBIN) {
			DaqItem &item = items[dev->pollIndex];
			std::vector<DATA_TYPE> values = dev->poll(item.id);
			item.updateValues(values);
			uint8_t nextIndex = (dev->pollIndex+1) % items.size();
			dev->pollIndex = nextIndex;
			state.logItem(item);
		}
		else if(dev->pollMode == PollMode::SINGLE_SHOT) {
			for(auto &item : items) {
				std::vector<DATA_TYPE> values = dev->poll(item.id);
				item.updateValues(values);
			}
			state.logItems(items);
		}
		dev->lastPoll = now;
	}
}