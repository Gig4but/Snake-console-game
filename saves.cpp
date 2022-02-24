#include "saves.h"
#include "snake.h"

static constexpr char savename[] = "save.hadik";

/*
Body1X Body1Y Body2X Body2Y ...
Obstacles1X Obstacles1Y Obstacles2X Obstacles2Y ...
Food1X Food1Y Food2X Food2Y ...
Score Direction SizeX SizeY MinSpeed ObstaclesLimit FoodLimit FoodRate FpsCounter 
*/
void Saves::SaveGame(std::string out) {
	remove(savename);
	std::ofstream savefile;
	savefile.open(savename);
	if (savefile.good()) {
		savefile << out;
		savefile.close();
	}
}


extern std::string load(std::vector<std::vector<unsigned int>> in);
std::string Saves::LoadGame() {
	std::ifstream savefile(savename);
	if (!savefile.good())
		return "save file doesn't exist near executable";
	std::vector<std::string> in = {};
	std::string line;
	while (std::getline(savefile, line))
		in.push_back(line);
	savefile.close();

	std::vector<std::vector<unsigned int>> converted;
	int anchor = 0;
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

	return load(converted);
}