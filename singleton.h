/************************************************
 * Hadik, console game "Snake" for Windows
 *
 * Created by Oleg Petruny in 2022
 * like credit program for course Programing C++
 *
 * License MIT
 ************************************************/

#pragma once

 /**
  * @brief Automatically creates singleton from class.
  * Use ::Get() to get an singleton instance.
  * Defines the default constructor.
  * Ovverides all assigns operators.
  * Leaves public access specifier behind.
  *
  * @param ClassName The class what needs to turn into singleton.
  */
#define SINGLETON(ClassName) \
	ClassName() = default; \
	ClassName(const ClassName&) = delete; \
	ClassName& operator=(const ClassName&) = delete; \
	ClassName(ClassName&&) = delete; \
	ClassName& operator=(ClassName&&) = delete; \
public: \
	static ClassName& Get() { \
		static ClassName* singleton = new ClassName(); \
		return *singleton; \
	}

  /**
	* @brief Automatically creates singleton from class.
	* Use ::Get() to get an singleton instance.
	* DOES NOT defines the default constructor.
	* Ovverides all assigns operators.
	* Leaves public access specifier behind.
	*
	* @param ClassName The class what needs to turn into singleton.
	*/
#define _SINGLETON(ClassName) \
	ClassName(); \
	ClassName(const ClassName&) = delete; \
	ClassName& operator=(const ClassName&) = delete; \
	ClassName(ClassName&&) = delete; \
	ClassName& operator=(ClassName&&) = delete; \
public: \
	static ClassName& Get() { \
		static ClassName* singleton = new ClassName(); \
		return *singleton; \
	}
