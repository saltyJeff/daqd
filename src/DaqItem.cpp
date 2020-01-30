#include "DaqItem.h"
#include "utils.h"

void DaqItem::writeToStream(std::ostream &stream) {
	stream << name << '|';
	int n = values.size();
	for (int i = 0; i < n; i++) {
		stream << values[i];
		if (i != n - 1) {
			stream << ',';
		}
	}
	stream << '|' << lastPoll << '\n';
}
void DaqItem::updateValues(const std::vector<DATA_TYPE>& newVals) {
	int n = newVals.size();
	values.resize(n);
	for (int i = 0; i < n; i++) {
		values[i] = newVals[i] * scale + offset; //y = mx+b
	}
	lastPoll = millis();
}