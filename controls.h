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
#include <map>
#include <memory>
#include <windows.h>
#include "singleton.h"

/**
 * @brief The safe function structure used instead of lambds or functions pointers.
 */
struct function { virtual void invoke() = 0; };

/**
 * @brief The map for key bindings, where key = keyboard key code and
 * value = (next key press delay, next key press timer, list of binded functions).
 */
using bindings = std::map<  int, std::tuple< unsigned int, unsigned int, std::vector<std::shared_ptr<function>> >   >;

/**
 * @brief Represents class for keyboard input processing.
 */
class Controls {
	static constexpr unsigned int defaultDelay = 150; //!< fixed default delay for nex key press
	bindings _dispetcher; //!< map of bindings

	SINGLETON(Controls);

	/**
	 * @brief The enum for easy convert/finding Windows virtual keyboard key codes 
	 * (see https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes)
	 */
	enum class Key: int {
		Enter = VK_RETURN,
		Up = VK_UP,
		Down = VK_DOWN,
		Left = VK_LEFT,
		Right = VK_RIGHT,
		Esc = VK_ESCAPE
	};

	/**
	 * @brief Bind a keyboard key press with function.
	 * 
	 * @param key The enum key name.
	 * @param func The overloaded function struct instance.
	 * @param delay The delay between next key press registration.
	 */
	void bind(Key key, std::shared_ptr<function> func, unsigned int delay = defaultDelay);

	/**
	 * @brief Clear all bindings.
	 */
	void removeAllBindgs();

	/**
	 * @brief Try all key in bindings list if pressed and call functions.
	 * 
	 * @param delta The delta time between frames or ticks.
	 * @param resetControls True if processed key press, clears all bindings
	 * right in processing time. After finding it true, function changes it
	 * to false and straight exits processing.
	 */
	void processInput(unsigned int delta, bool& resetControls);
};
