#pragma once
#pragma warning(disable:26812)

#include <string>
#include <vector>
#include <windows.h>

// A,B where A text color and B background color
enum Color :int {
	WhiteBlack = 7,
	RedBlack = 4,
	GreenBlack = 10,
	BlueBlack = 9,
	YellowBlack = 6,
	BlackWhite = 112,
	BlackRed = 192
};

//string "pixel"
struct sixel {
	std::string content = "";
	Color color = Color::WhiteBlack;

	sixel(std::string content) : content(content) {}
	sixel(std::string content, Color color) : content(content), color(color) {}
};

class Console {
    const HANDLE _hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

public:
	std::vector<std::vector<sixel>> buffer = {};

	Console();

    void clear();
	void update();
    void setColor(Color c);
	void setCursorPosition(int x, int y);
};
