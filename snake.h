/************************************************
 * Hadik, console game "Snake" for Windows
 *
 * Created by Oleg Petruny in 2022
 * like credit program for course Programing C++
 *
 * License MIT
 ************************************************/

#pragma once

#include <vector>
#include <random>
#include "globals.h"
#include "graphics.h"
#include "controls.h"

/**
 * @brief Represents namespace with defined chars for snake game drawing.
 */
namespace snakestyle {
	static constexpr char border = '#'; //!< char of map border (bushes)
	static constexpr char head = 'O'; //!< char of snake head
	static constexpr char body = 'X'; //!< char of snake body
	static constexpr char tail = 'x'; //!< char of snake tail
	static constexpr char fat = 'H'; //!< char of snake food in snake body
	static constexpr char food[3] = { '$', '&', '@' }; //!< chars of food
	static constexpr char obstacles[2] = { '*', '+' }; //!< chars of obstacles
}

/**
 * @brief Represents class of playable game.
 */
class Snake {
	std::vector<object> body; //!< array of snake body pieces on map
	std::vector<object> food; //!< array of food on map
	std::vector<object> obstacles; //!< array of obstacles on map
	
	unsigned int waiter = 0; //!< counter to the next snake move
	bool alive = false; //!< indicates if snake is alive
	std::unique_ptr<function> death; //!< pointer to function invoked after player death

	/**
	 * @brief Checks the objects collisions.
	 * 
	 * @param growth Indicates if snake eated something.
	 * 
	 * @return bool true if a collision caused the death, false if alive or eated.
	 */
	bool collisions(bool& growth);

public:
	Direction dir; //!< indicates snake move direction
	unsigned int score; //!< indicates eated food

	/**
	 * @brief Generates random uint in specified range (both values included).
	 * 
	 * @param from Generates from.
	 * @param to Generates to.
	 * 
	 * @return unsigned int generated value.
	 */
	static unsigned int Rand(unsigned int from, unsigned int to);

	/**
	 * @brief Default constructor. Still needs pointer to death function.
	 */
	Snake();

	/**
	 * @brief Common constructor. Moves death function pointer and invokes default constructor.
	 */
	Snake(std::unique_ptr<function> death);

	/**
	 * @brief Advanced constructor. Creates snake from passed parameters.
	 */
	Snake(std::vector<object>& body, std::vector<object>& food, std::vector<object>& obstacles, Direction& direction, unsigned int score, std::unique_ptr<function> death);

	/**
	 * @brief Generates map with objects and exports it to passed buffer.
	 *
	 * @param buffer Array for map export.
	 */
	void Draw(std::vector<std::vector<sixel>>& buffer);
	


	/**
	 * @brief Calculates the full snake body move by one cell.
	 *
	 * @return bool true if move was successful.
	 */
	bool move();

	/**
	 * @brief Changes direction of snake move.
	 * 
	 * @param d Needed direction.
	 */
	void move(Direction d) { dir = d; }

	/**
	 * @brief Places new food on map.
	 */
	void spawnFood();

	/**
	 * @brief Exports all game objects in string.
	 * In format:
	 * "body[0].x body[0].y ... body[n].x body[n].y\n
	 * obstacles[0].x obstacles[0].y ... obstacles[n].x obstacles[n].y\n
	 * food[0].x food[0].y ... food[n].x food[n].y\n
	 * score (int)direction"
	 * 
	 * @return std::string of all game objects.
	 */
	std::string ToString();
};
