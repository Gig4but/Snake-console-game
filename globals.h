/************************************************
 * Hadik, console game "Snake" for Windows
 *
 * Created by Oleg Petruny in 2022
 * like credit program for course Programing C++
 *
 * License MIT
 ************************************************/

#pragma once

#include <utility>
#include "singleton.h"

//* Structs & Enums *****************************

/**
 * @brief The struct for work with two coordinates.
 * Has only == bool operator.
 */
struct vector2D {
	int x, y;
	
	/**
	* @brief Creates vector2D(0, 0).
	*/
	vector2D() : x(0), y(0) {}

	/**
	* @brief Creates vector2D(x, x).
	*/
	vector2D(int x) : x(x), y(x) {}

	/**
	* @brief Creates vector2D(x, y).
	*/
	vector2D(int x, int y) : x(x), y(y) {}

	vector2D& operator=(const vector2D& other);
	bool operator==(const vector2D& other) { return this->x == other.x && this->y == other.y; }
};

/**
 * @brief The modification of struct vector2D,
 * what in addition to 2 coordinates contain one char over.
 * Has only == bool operator, what compares only coordinates, not chars.
 */
struct object :public vector2D {
	char c;
	object& operator=(const vector2D& other) { return *this = object(this->c, other.x, other.y); }
	object& operator=(const object& other);
	bool operator==(const vector2D& other) { return this->x == other.x && this->y == other.y; }
	bool operator==(const object& other) { return *this == vector2D(other.x, other.y); }

	/**
	* @brief Creates object(char, x, y).
	*/
	object(char c, vector2D v) : c(c), vector2D(v.x, v.y) {}

	/**
	* @brief Creates object(char, x, y).
	*/
	object(char c, int x, int y) : c(c), vector2D(x, y) {}

	/**
	* @brief Creates object(char, x, x).
	*/
	object(char c, int x) : c(c), vector2D(x) {}

	/**
	* @brief Creates object(char, 0, 0).
	*/
	object(char c) : c(c), vector2D(0) {}
};

/**
 * @brief The enum for direction specification.
 */
enum class Direction {
	Up = 0,
	Down = 1,
	Left = 2,
	Right = 3
};

/**
 * @brief The struct for declaration custom range of any type.
 */
template<typename T>
struct range {
	T Min, Max;

	/**
	* @brief Creates range(int 0, int 1).
	*/
	range() : Min(0), Max(1) {}

	/**
	* @brief Creates range(int 0, int max).
	*/
	range(int max) : Min(0), Max(max) {}

	/**
	* @brief Creates range(T min, T max).
	*/
	range(T min, T max) : Min(max), Max(max) {}
};


//* Game Properties *****************************

/**
 * @brief Represents engine environment variables.
 */
class GameEnvironment {
	SINGLETON(GameEnvironment);

	bool run = true; //!< main loop (game) state
	std::pair<time_t, time_t> timer = { 0, 0 }; //!< time stamp between ticks/frames {last, new}
	const unsigned int fpsLimit = 45; //!< the maximum of frames per second
	const unsigned int tickDelay = 1000/fpsLimit; //!< delay between ticks in miliseconds
	unsigned int tick = 0; //!< tick counter
	unsigned int sec = 0; //!< counter to one second 0 - 1000
	unsigned int tps = fpsLimit; //!< the maximum of ticks per second
	unsigned int tickDelta = 0; //!< delta time in miliseconds between last and new ticks
	bool resetControls = false; //!< special variable see Controls::processInput()
};

/**
 * @brief Represents game settings variables.
 */
class GameSettings {
	SINGLETON(GameSettings);

	const float randomFinest = 1.0; //!< rand used like "try generate smaller value then upper bound", so this needed for scale smaller value
	vector2D size = { 16, 16 }; //!< the size of game map
	const range<vector2D> sizeRange = { {5, 5}, {63, 63} }; //!< the min/max size of game map
	unsigned int startSpeed = 5; //!< the start speed of snake
	const range<unsigned int> startSpeedRange = { 2, 20 }; //!< the min/max speed of snake
	unsigned int obstaclesLimit = size.x/4; //!< the maximal count of obstacles on map
	const range<unsigned int> obstaclesLimitRange = { 0, 63 }; //!< the min/max count of obstacles on map
	unsigned int foodLimit = 3; //!< the maximal count of food on map
	const range<unsigned int> foodLimitRange = { 1, 63 }; //!< the min/max count of food on map
	unsigned int foodRate = 20; //!< the upper bound multiplied with randomFinest and compared with rand(0, 100) for food generation on map
	const range<unsigned int> foodRateRange = { 2, 100 }; //!< the min/max upper bound multiplied with randomFinest and compared with rand(0, 100) for food generation on map
	bool fpscounter = false; //!< the visibility of fps counter in console
};

/**
 * @brief Represents functions for engine manipulation.
 */
class GameFunctions{
	//SINGLETON(GameFunctions);
public:
	/**
	 * @brief Closes the game.
	 */
	static void Exit() { GameEnvironment::Get().run = false; }
};
