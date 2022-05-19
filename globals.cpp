/************************************************
 * Hadik, console game "Snake" for Windows
 *
 * Created by Oleg Petruny in 2022
 * like credit program for course Programing C++
 *
 * License MIT
 ************************************************/

#include "globals.h"

vector2D& vector2D::operator=(const vector2D& other) {
	if (this == &other)
		return *this;

	this->x = other.x;
	this->y = other.y;

	return *this;
}

object& object::operator=(const object& other) {
	if (this == &other)
		return *this;

	this->x = other.x;
	this->y = other.y;
	this->c = other.c;

	return *this;
}
