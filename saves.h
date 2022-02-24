#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>

struct Saves {
	static std::string LoadGame();
	static void SaveGame(std::string obj);
};