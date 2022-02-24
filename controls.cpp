#include "controls.h"

void Controls::bind(Key key, function* func, int delay) {
	_dispetcher[key].second.push_back(std::make_shared<function*>(func));
	_dispetcher[key].first.first = delay;
	_dispetcher[key].first.second = 0;
}

void Controls::removeAllBindgs() {
	_dispetcher.clear();
}

int Controls::ProcessInput(unsigned int delta) {
	for (auto bind = _dispetcher.begin(); bind != _dispetcher.end(); bind++) {
		if (GetAsyncKeyState(bind->first) && bind->second.first.first < bind->second.first.second+delta) {
			bind->second.first.second = 0;
			for (size_t i = 0; i < bind->second.second.size(); i++) {
				(*bind->second.second[i])->invoke();
				if (resetControls) {
					resetControls = false;
					return 1;
				}
			}
		} else {
			bind->second.first.second += delta;
		}
	}
	return 0;
}