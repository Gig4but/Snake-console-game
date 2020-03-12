#include "controls.h"

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

snake::snake() {
	coords = new coordinates[1];
}

void snake::extend(int &points, int key) {
	extendLength(points);
	if (points > 1) {
		if (coords[points - 1].x == coords[points - 2].x) {
			coords[points].x = coords[points - 1].x;
			if (coords[points - 1].y < coords[points - 2].y) {
				coords[points].y = coords[points - 1].y - 1;
			} else if (coords[points - 1].y > coords[points - 2].y) {
				coords[points].y = coords[points - 1].y + 1;
			}
		} else if (coords[points - 1].y == coords[points - 2].y) {
			coords[points].y = coords[points - 1].y;
			if (coords[points - 1].x < coords[points - 2].x) {
				coords[points].x = coords[points - 1].x - 1;
			} else if (coords[points - 1].x > coords[points - 2].x) {
				coords[points].x = coords[points - 1].x + 1;
			}
		}
	} else {
		if (key == KEYUP) {
			coords[points].x = coords[points - 1].x;
			coords[points].y = coords[points - 1].y + 1;
		} else if (key == KEYDOWN) {
			coords[points].x = coords[points - 1].x;
			coords[points].y = coords[points - 1].y - 1;
		}
		if (key == KEYLEFT) {
			coords[points].x = coords[points - 1].x + 1;
			coords[points].y = coords[points - 1].y;
		} else if (key == KEYRIGHT) {
			coords[points].x = coords[points - 1].x - 1;
			coords[points].y = coords[points - 1].y;
		}
	}
}

void snake::extendLength(int &points) {
	points++;
	coordinates *buffer = new coordinates[points];
	for (int i = 0; i < (points); i++) {
		buffer[i] = coords[i];
	}
	delete[] coords;
	coords = new coordinates[points + 1];
	for (int i = 0; i < points; i++) {
		coords[i] = buffer[i];
	}
	delete[] buffer;
}