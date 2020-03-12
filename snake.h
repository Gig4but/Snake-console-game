#pragma once

struct coordinates {
	int x, y;
};

class snake {
public:
	coordinates *coords;
	snake();
	void extend(int &points, int key);
private:
	void extendLength(int &points);
};