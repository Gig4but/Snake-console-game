#include "snake.h"

vector2D& vector2D::operator=(const vector2D& other) {
	if (this == &other)
		return *this;

	this->x = other.x;
	this->y = other.y;

	return *this;
}

object& object::operator=(const object& other) {
	if (this == &other)
		return *this;

	this->x = other.x;
	this->y = other.y;
	this->c = other.c;

	return *this;
}

using Rint = std::uniform_int_distribution<std::mt19937::result_type>;
int Snake::Rand(int from, int to) {
	static std::random_device dev;
	static std::mt19937 rng(dev());
	Rint fromto(from, to);
	return (fromto(rng));
}

Snake::Snake() {
	dir = Direction::Up;
	alive = true;

	//set head and tail
	body[0] = object(snakestyle::head, Rand(1, size.x-1), Rand(1, size.y));
	body[1] = object(snakestyle::tail, body[0].x, body[0].y+1);

	vector2D buffer = {0};

	//generate obstacles
	for (int i = 0; i < (int)obstaclesLimit; i++) {
		buffer = vector2D(Rand(1, size.x-1), Rand(1, size.y-1));
		if (buffer == body[0] || buffer == body[1]) {
			i--;
			continue;
		}
		obstacles.push_back(object(snakestyle::obstacles[Rand(0, sizeof(snakestyle::obstacles) / sizeof(char) - 1)], buffer));
	}

	//generate obstacles
	spawnFood();
}
Snake::Snake(std::vector<object>& body, std::vector<object>& food, std::vector<object>& obstacles, unsigned int score, Direction direction) {
	dir = direction;
	alive = true;
	this->score = score;

	this->body = body;
	this->body[0].c = snakestyle::head;
	for (size_t i = 1; i < this->body.size()-1; i++) {
		this->body[i].c = snakestyle::body;
	}
	this->body[body.size()-1].c = snakestyle::tail;

	this->obstacles = obstacles;
	for (size_t i = 0; i < this->obstacles.size(); i++) {
		this->obstacles[i].c = snakestyle::obstacles[Rand(0, sizeof(snakestyle::obstacles) / sizeof(char) - 1)];
	}
	
	this->food = food;
	for (size_t i = 0; i < this->food.size(); i++) {
		this->food[i].c = snakestyle::food[Rand(0, sizeof(snakestyle::food) / sizeof(char) - 1)];
	}
}

//copy-paste loops reduction
std::string _export(std::vector<object>& what) {
	std::string out;
	for (size_t i = 0; i < what.size(); i++) {
		out += std::to_string(what[i].x) + " " + std::to_string(what[i].y) + " ";
		if (i == what.size()-1)
			out = out.substr(0, out.size()-1) + " \n";
	}
	return out;
}

std::string Snake::saveexport() {
	return _export(body) + _export(obstacles) + _export(food) + std::to_string(score) + " " + std::to_string((int)dir) + " ";
}

bool Snake::colisions(bool& growth) {
	for (int i = 0; i < obstacles.size(); i++) {
		if (body[0] == obstacles[i]) {
			Death();
			return true;
		}
	}
	for (int i = 1; i < body.size(); i++) {
		if (body[0] == body[i]) {
			Death();
			return true;
		}
	}
	for (int i = 0; i < food.size(); i++) {
		if (body[0] == food[i]) {
			body[1].c = snakestyle::fat;
			score++;
			food.erase(food.begin() + i);
			growth = false;
		}
	}

	return false;
}

bool Snake::move() {
	object buffer[2] = {body[0], body[0]};

	switch (dir) {
		case Direction::Up:
			if (body[0].y > 0)
				body[0].y--;
			else
				body[0].y = size.y-1;
			break;
		case Direction::Down:
			if (body[0].y < size.y-1)
				body[0].y++;
			else
				body[0].y = 0;
			break;
		case Direction::Left:
			if (body[0].x > 0)
				body[0].x--;
			else
				body[0].x = size.x-1;
			break;
		case Direction::Right:
			if (body[0].x < size.x-1)
				body[0].x++;
			else
				body[0].x = 0;
			break;
		default:
			return false;
	}

	bool growth = true;
	if (colisions(growth)) {
		alive = false;
		return false;
	}
	
	
	for (int i = 1; i < body.size(); i++) {
		if (body[i-1].c == snakestyle::fat && growth) {
			growth = false;
			body[i-1].c = snakestyle::body;
			body[i].c = snakestyle::fat;
		}
		buffer[1] = body[i];
		body[i] = vector2D(buffer[0].x, buffer[0].y);
		buffer[0] = buffer[1];
		if (i == body.size()-1 && body[i].c == snakestyle::fat && growth) {
			growth = false;
			body[i].c = snakestyle::body;
			body.push_back(object(snakestyle::tail, buffer[0].x, buffer[0].y));
			break;
		}
	}

	return true;
}

void Snake::spawnFood() {
	vector2D buffer;
	bool duplicate = false;
	while (true) {
		buffer = { Rand(1, size.x-1), Rand(1, size.y-1) };
		for (auto it = food.begin(); it != food.end(); it++)
			if (*it == buffer)
				duplicate = true;
		if (!duplicate) {
			food.push_back(object(snakestyle::food[Rand(0, sizeof(snakestyle::food) / sizeof(char) - 1)], buffer));
			break;
		}
	}
}

void Snake::Draw(std::vector<std::vector<sixel>>& buffer) {
	if (waiter < (tps-score)) {
		waiter++;
	} else {
		waiter = 0;
		move();

		if (food.size() < foodLimit && Rand(0, 100) < foodRate) {
			spawnFood();
		}
	}

	
	if (alive) {
		size_t firstrow = buffer.size();
		for (int y = 0; y < size.y+1; y++) {
			buffer.push_back({});
			for (int x = 0; x < size.x+1; x++) {
				buffer[firstrow+y].push_back(sixel(" "));
			}
		}

		for (int y = 1; y < buffer.size()-1; y++) {
			buffer[firstrow+y][0].content = snakestyle::border;
			buffer[firstrow+y][buffer[firstrow+y].size()-1].content = snakestyle::border;
		}
		for (int x = 0; x < buffer[firstrow].size(); x++) {
			buffer[firstrow][x].content = snakestyle::border;
			buffer[buffer.size()-1][x].content = snakestyle::border;
		}

		for (auto it = body.begin(); it != body.end(); it++) {
			buffer[it->y][it->x].content = it->c;
			buffer[it->y][it->x].color = Color::GreenBlack;
		}
		for (auto it = obstacles.begin(); it != obstacles.end(); it++) {
			buffer[it->y][it->x].content = it->c;
			buffer[it->y][it->x].color = Color::BlackRed;
		}
		for (auto it = food.begin(); it != food.end(); it++) {
			buffer[it->y][it->x].content = it->c;
			switch (it->c) {
				case snakestyle::food[0]:
					buffer[it->y][it->x].color = Color::RedBlack;
					break;
				case snakestyle::food[1]:
					buffer[it->y][it->x].color = Color::YellowBlack;
					break;
				case snakestyle::food[2]:
					buffer[it->y][it->x].color = Color::BlueBlack;
					break;
			}
		}
		buffer.push_back({ sixel("SCORE: " + std::to_string(score)) });
	}
}