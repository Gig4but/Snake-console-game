/************************************************
 * Hadik, console game "Snake" for Windows
 *
 * Created by Oleg Petruny in 2022
 * like credit program for course Programing C++
 *
 * License MIT
 ************************************************/

#include "saves.h"

static constexpr char savename[] = "save.hadik";

void Saves::Save(std::string obj) {
	static GameSettings& Sett = GameSettings::Get();
	obj += std::to_string(Sett.size.x) + " " + std::to_string(Sett.size.y) + " " \
		+ std::to_string(Sett.startSpeed) + " " + std::to_string(Sett.obstaclesLimit) + " " \
		+ std::to_string(Sett.foodLimit) + " " + std::to_string(Sett.foodRate) + " " \
		+ (Sett.fpscounter ? "1" : "0") + " \n";
	
	remove(savename);
	std::ofstream savefile;
	savefile.open(savename);

	if (savefile.good()) {
		savefile << obj;
		savefile.close();
	}
}

std::string _load(std::vector<std::vector<unsigned int>>& in, std::vector<object>& body, std::vector<object>& food, std::vector<object>& obstacles, Direction& direction, unsigned int& score) {
	for (size_t i = 0; i < in[0].size(); i += 2) {
		body.push_back(object('\0', vector2D(in[0][i], in[0][i+1])));
	}
	for (size_t i = 0; i < in[1].size(); i += 2) {
		obstacles.push_back(object('\0', vector2D(in[1][i], in[1][i+1])));
	}
	for (size_t i = 0; i < in[2].size(); i += 2) {
		food.push_back(object('\0', vector2D(in[2][i], in[2][i+1])));
	}

	static GameSettings& Sett = GameSettings::Get();

	score = in[3][0];
	direction = (Direction)in[3][1];
	Sett.size = vector2D(in[3][2], in[3][3]);
	Sett.startSpeed = in[3][4];
	Sett.obstaclesLimit = in[3][5];
	Sett.foodLimit = in[3][6];
	Sett.foodRate = in[3][7];
	Sett.fpscounter = in[3][8];

	return "";
}

std::string Saves::Load(std::vector<object>& body, std::vector<object>& food, std::vector<object>& obstacles, Direction& direction, unsigned int& score) {
	std::ifstream savefile(savename);
	if (!savefile.is_open() && !savefile.good())
		return "save file doesn't exist near executable";
	std::vector<std::string> in = {};
	std::string line;
	while (std::getline(savefile, line))
		in.push_back(line);
	savefile.close();

	std::vector<std::vector<unsigned int>> converted;
	size_t anchor = 0;
	unsigned int n;
	for (size_t i = 0; i < in.size(); i++) {
		converted.push_back({});
		for (size_t cursor = 1; cursor < in[i].length(); cursor++) {
			while (!std::isspace(in[i][cursor]) && cursor < in[i].length())
				cursor++;
			n = atoi(in[i].substr(anchor, cursor-anchor).c_str());
			if (n == 0 && in[i][anchor] != '0')
				return "save file is corupted";
			converted[i].push_back(n);
			cursor++;
			anchor = cursor;
		}
		anchor = 0;
	}

	return _load(converted, body, obstacles, food, direction, score);
}
