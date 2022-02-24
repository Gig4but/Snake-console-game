#pragma warning(disable:4244)
#include <iostream>
#include <windows.h>
#include <memory>
#include <chrono>
#include <thread>
#include "graphic.h"
#include "menu.h"
#include "saves.h"
#include "snake.h"

/* GLOBALS */
//Graphic
Console cmd;
std::shared_ptr<Menu*> scene;
//Input
Controls input = Controls();
//Game Environment
bool run = true;
std::pair<time_t, time_t> timer = { 0, 0 }; // new, last
constexpr unsigned int fps_limit = 45;
constexpr unsigned int tickDelay = 1000/fps_limit; //in ms
unsigned int tick = 0; //tick counter
unsigned int sec = 0; //second: 0 - 1000
unsigned int tps = fps_limit; //ticks per second
unsigned int tickDelta = 0;
bool resetControls = false;
//Game Settings
vector2D size = { 16, 16 };
unsigned int minspeed = 5;
unsigned int obstaclesLimit = size.x/4;
unsigned int foodLimit = 3;
unsigned int foodRate = 20;
bool fpscounter = false;

using namespace std::chrono;

/* Main loop */
int main() {
	//Initialize first screen
	scene = std::make_shared<Menu*>(new Menus::Home());
	(*scene)->AfterConstructor();

	while (run) {
		//stabilization
		timer.second = timer.first;
		timer.first = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		tickDelta = timer.first - timer.second;
		
		tick++;
		sec += tickDelta;

		if (tickDelta < tickDelay)
			Sleep(tickDelay - tickDelta);

		if (sec > 1000) {
			tps = tick;
			tick = 0;
			sec = 0;
		}

		//graphic
		(*scene)->Draw(cmd.buffer);
		if (fpscounter)
			cmd.buffer.push_back({ sixel("\nFPS: " + std::to_string(tps)) });
		//cmd.buffer.push_back({ sixel("\nsecond: " + std::to_string(((float)sec)/1000) + "        ")});
		cmd.update();

		//input
		input.ProcessInput(tickDelta);
	}

	return 0;
}

void death(unsigned int score) {
	(*scene)->change(std::make_shared<Menu*>(new Menus::Death(score)));
}
void save(std::string out) {
	out += std::to_string(size.x) + " " + std::to_string(size.y) + " " \
		+ std::to_string(minspeed) + " " + std::to_string(obstaclesLimit) + " " \
		+ std::to_string(foodLimit) + " " + std::to_string(foodRate) + " " \
		+ (fpscounter ? "1" : "0") + " \n";
	Saves::SaveGame(out);
}
std::string load(std::vector<std::vector<unsigned int>> in) {
	std::vector<object>* body = new std::vector<object>();
	std::vector<object>* obstacles = new std::vector<object>();
	std::vector<object>* food = new std::vector<object>();
	Direction dir;
	unsigned int score;
	for (size_t i = 0; i < in[0].size(); i += 2) {
		body->push_back(object('\0', vector2D(in[0][i], in[0][i+1])));
	}
	for (size_t i = 0; i < in[1].size(); i += 2) {
		obstacles->push_back(object('\0', vector2D(in[1][i], in[1][i+1])));
	}
	for (size_t i = 0; i < in[2].size(); i += 2) {
		food->push_back(object('\0', vector2D(in[2][i], in[2][i+1])));
	}
	score = in[3][0];
	dir = (Direction)in[3][1];
	size = vector2D(in[3][2], in[3][3]);
	minspeed = in[3][4];
	obstaclesLimit = in[3][5];
	foodLimit = in[3][6];
	foodRate = in[3][7];
	fpscounter = in[3][8];

	(*scene)->change(std::make_shared<Menu*>(new Menus::Game(body, obstacles, food, score, dir)));

	return "";
}
void exit() {
	run = false;
	//Saves::SaveGame();
}