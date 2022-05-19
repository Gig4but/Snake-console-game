/************************************************
 * Hadik, console game "Snake" for Windows
 *
 * Created by Oleg Petruny in 2022
 * like credit program for course Programing C++
 *
 * License MIT
 ************************************************/

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "singleton.h"

/**
 * @brief The enum for convert windows console color to human readable.
 * Two colors in name, where first one specifies the color of text, the second - background.
 */
enum class Color :int {
	WhiteBlack = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	RedBlack = FOREGROUND_RED,
	GreenBlack = FOREGROUND_GREEN,
	BlueBlack = FOREGROUND_BLUE,
	YellowBlack = FOREGROUND_RED | FOREGROUND_GREEN,
	BlackWhite = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	BlackRed = BACKGROUND_RED
};

/**
 * @brief The struct for save printable content and his color.
 * For space saving (to not duplicate color for many characters) uses string instead of char.
 * From words string + pixel = sixel.
 */
struct sixel {
	std::string content = ""; //!< content of sixel
	Color color = Color::WhiteBlack; //!< color of content

	/**
	 * @brief Creates sixel with Color::WhiteBlack content.
	 */
	sixel(std::string content) : content(std::move(content)) {}

	/**
	 * @brief Creates sixel with colored content.
	 */
	sixel(std::string content, Color color) : content(std::move(content)), color(color) {}
};

/**
 * @brief Represents class for work with windows console.
 */
class Console {
	const HANDLE _hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //!< windows handle for comunication with console process

	_SINGLETON(Console);

	std::vector<std::vector<sixel>> buffer = {}; //!< buffer to print

	/**
	 * @brief Clears the full console.
	 */
	void clear();

	/**
	 * @brief Replaces actual console content by buffer content and flush the buffer.
	 */
	void update();

	/**
	 * @brief Sets console output chars color (need before printing).
	 */
	void setColor(Color c);

	/**
	 * @brief Sets position of cursor in console where printing starts.
	 */
	void setCursorPosition(size_t x, size_t y);
};
