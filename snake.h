#pragma once
#include <vector>
#include <random>
#include "graphic.h"
#include "saves.h"

namespace snakestyle {
	static constexpr char border = '#';
	static constexpr char head = 'O';
	static constexpr char body = 'X';
	static constexpr char tail = 'x';
	static constexpr char fat = 'H';
	static constexpr char food[3] = { '$', '&', '@' };
	static constexpr char obstacles[2] = { '*', '+' };
}

enum Direction { 
	Up = 0, 
	Down = 1, 
	Left = 2, 
	Right = 3
};

struct vector2D {
	int x, y;
	vector2D() { x = y = 0; }
	vector2D(int x) : x(x), y(x) {}
	vector2D(int x, int y) : x(x), y(y) {}

	vector2D& operator=(const vector2D& other);

	bool operator==(const vector2D& other) { return this->x == other.x && this->y == other.y; }
};

struct object :public vector2D { 
	char c; 
	object& operator=(const vector2D& other) { return *this = object(this->c, other.x, other.y); }
	object& operator=(const object& other);
	bool operator==(const vector2D& other) { return this->x == other.x && this->y == other.y; }
	bool operator==(const object& other) { return *this == vector2D(other.x, other.y); }

	object(char c, vector2D v) : c(c), vector2D(v.x, v.y) {}
	object(char c, int x, int y) : c(c), vector2D(x, y) {}
	object(char c, int x) : c(c), vector2D(x) {}
	object(char c) : c(c), vector2D(0) {}
};

extern unsigned int tps;
extern unsigned int speed;
extern vector2D size;
extern unsigned int obstaclesLimit;
extern unsigned int foodLimit;
extern unsigned int foodRate;
extern void death(unsigned int score);
extern void save(std::string out);

//main code of game
class Snake {
	std::vector<object> body = { object(snakestyle::head), object(snakestyle::tail, 0, 1)};
	std::vector<object> food = {};
	std::vector<object> obstacles = {};
	
	unsigned int waiter = 0;
	bool alive = false;

	bool colisions(bool& growth);
	//export main snake variables into string
	std::string saveexport();

public:
	Direction dir;
	unsigned int score = 0;

	static int Rand(int from, int to);

	Snake();
	Snake(std::vector<object>& body, std::vector<object>& food, std::vector<object>& obstacles, unsigned int score, Direction direction);

	void Draw(std::vector<std::vector<sixel>>& buffer);
	
	bool move();
	//yep, it's only for direction change
	void move(Direction d) { dir = d; }
	void spawnFood();

	void Death() { death(score); }
	void Save() { save(saveexport()); }
};