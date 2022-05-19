/************************************************
 * Hadik, console game "Snake" for Windows
 *
 * Created by Oleg Petruny in 2022
 * like credit program for course Programing C++
 *
 * License MIT
 ************************************************/

#include "controls.h"

void Controls::bind(Key key, std::shared_ptr<function> func, unsigned int delay) {
	auto& bind = _dispetcher[(int)key];
	std::get<0>(bind) = delay;
	std::get<1>(bind) = 0;
	std::get<2>(bind).push_back(func);
}

void Controls::removeAllBindgs() {
	_dispetcher.clear();
}

void Controls::processInput(unsigned int delta, bool& resetControls) {
	for (auto bind = _dispetcher.begin(); bind != _dispetcher.end(); bind++) {
		if (GetAsyncKeyState(bind->first) && std::get<0>(bind->second) < std::get<1>(bind->second) + delta) {
			std::get<1>(bind->second) = 0;
			for (size_t i = 0; i < std::get<2>(bind->second).size(); i++) {
				std::get<2>(bind->second)[i]->invoke();
				if (resetControls) {
					resetControls = false;
					return;
				}
			}
		} else {
			std::get<1>(bind->second) += delta;
		}
	}
	return;
}
