/************************************************
 * Hadik, console game "Snake" for Windows
 *
 * Created by Oleg Petruny in 2022
 * like credit program for course Programing C++
 *
 * License MIT
 ************************************************/

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include "globals.h"

/**
 * @brief Represents struct for work game saves.
 */
struct Saves {
	/**
	 * @brief Reads game save file and loads data into passed variables.
	 *
	 * @param body The array for snake body (loads only position of body pieces).
	 * @param food The array for food positions on map.
	 * @param delay The array for obstacles positions on map.
	 * @param direction The direction of snake movement.
	 * @param score The actual score of game.
	 * 
	 * @return std::string message of error (if "" == load OK)
	 */
	static std::string Load(std::vector<object>& body, std::vector<object>& food, std::vector<object>& obstacles, Direction& direction, unsigned int& score);

	/**
	 * @brief Writes game into save file near executable.
	 *
	 * @param obj The string obtained from Snake::ToString().
	 */
	static void Save(std::string obj);
};

/* Actual format of .save file ******************
 * 
 * Body1X Body1Y Body2X Body2Y ...
 * Obstacles1X Obstacles1Y Obstacles2X Obstacles2Y ...
 * Food1X Food1Y Food2X Food2Y ...
 * Score Direction SizeX SizeY MinSpeed ObstaclesLimit FoodLimit FoodRate FpsCounter
 * 
 ************************************************/