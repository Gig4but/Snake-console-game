#pragma once
#pragma warning(disable:26812)

#include <vector>
#include <map>
#include <memory>
#include <windows.h>

struct function { virtual void invoke() = 0; };
//using bindings = std::map<int, std::map<const Menu*, std::vector<void (*)()>>>;
//map of KEY - ( (delay, time), {functions} )
using bindings = std::map<int, std::pair<std::pair<unsigned int, unsigned int>, std::vector<std::shared_ptr<function*>>>>;

extern bool resetControls;

class Controls {
public:
	enum Key: int {
		Enter = VK_RETURN,
		Up = VK_UP,
		Down = VK_DOWN,
		Left = VK_LEFT,
		Right = VK_RIGHT,
		Esc = VK_ESCAPE
	};

	void bind(Key key, function* func, int delay = 150);
	void removeAllBindgs();
	int ProcessInput(unsigned int delta);

private:
	bindings _dispetcher = bindings();
};