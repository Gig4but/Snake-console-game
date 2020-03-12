#include <iostream>
#include <ctime>
#include <windows.h>

#include "game.h"
#include "snake.h"
#include "controls.h"

//settings
int delayS = 200;
int sizeS = 20;
char headS = '#';
char bodyS = 'z';
char foodS = '$';

int main() {
menu:
	while (true) {
		system("cls");
		std::cout << "[1] Start\n"
				  << "[2] Settings\n"
				  << "[3] Credits\n"
				  << "[4] Exit\n"
				  << "Type the number: ";
		int in = 0;
		std::cin >> in;
		switch (in) {
			case 1:
				goto start;
			case 2:
				while (true) {
					system("cls");
					std::cout << "[1] Change delay time\n"
							  << "[2] Change game array size\n"
							  << "[3] Change snake head symbol\n"
							  << "[4] Change snake body symbol\n"
							  << "[5] Change food symbol\n"
							  << "[6] Go back\n"
							  << "Type the number: ";
					std::cin >> in;
					switch (in) {
						case 1:
							std::cout << "Type new timinig delay (in ms (default: 200)): ";
							std::cin >> delayS;
							break;
						case 2:
							std::cout << "Type new game array size (default: 20^2): ";
							std::cin >> sizeS;
							break;
						case 3:
							std::cout << "Type new snake head symbol (default: #): ";
							std::cin >> headS;
							break;
						case 4:
							std::cout << "Type new snake body symbol (default: z): ";
							std::cin >> bodyS;
							break;
						case 5:
							std::cout << "Type new food symbol (default: $): ";
							std::cin >> foodS;
							break;
						case 6:
							goto menu;
						default:
							break;
					}
				}
				break;
			case 3:
				std::cout << "Code:  Oleg Petruny\tDesign:    Oleg Petruny\n"
						  << "Sound: Oleg Petruny\tMarketing: Oleg Petruny\n"
						  << "Special thanks to: Oleg Petruny\n";
				system("pause");
				break;
			case 4:
				return 0;
			default:
				break;
		}
	}
start:
	srand(time(0));

	snake pc; //create playable character and set first coords
	pc.coords[0].x = 1 + rand() % (sizeS-2);
	pc.coords[0].y = 1 + rand() % (sizeS-2);
	area field;
	coordinates food; //creat coords for food
	food.x = 1 + rand() % (sizeS-2);
	food.y = 1 + rand() % (sizeS-2);

	int points = 0; //snake length
	int key = 0;   //keyboard key
	int xer = 0;  //x mover
	int yer = 0; //y mover



	while (true) {
		if (pc.coords[0].x == food.x && pc.coords[0].y == food.y) { //test if player eat
			pc.extend(points, key);
			do {
				food.x = 1 + rand() % (sizeS-2);
				food.y = 1 + rand() % (sizeS-2);
			} while (pc.coords[0].x == food.x && pc.coords[0].y == food.y);
		}

		std::cout << "Points: " << points << "\n";
		field.array[pc.coords[0].y][pc.coords[0].x] = headS; //write HEAD to game array
		for (int i = 1; i <= points; i++) { //write BODYs to game array
			field.array[pc.coords[i].y][pc.coords[i].x] = bodyS;
		}
		field.array[food.y][food.x] = foodS; //write FOOD to game array
		std::cout << field.draw();

		if (GetAsyncKeyState(KEYUP) || key == KEYUP) { //test holden button
			key = KEYUP;
			xer = 0;
			yer = -1;
		} else if (GetAsyncKeyState(KEYDOWN)  || key == KEYDOWN) {
			key = KEYDOWN;
			xer = 0;
			yer = 1;
		}
		if (GetAsyncKeyState(KEYLEFT)  || key == KEYLEFT) {
			key = KEYLEFT;
			xer = -1;
			yer = 0;
		} else if (GetAsyncKeyState(KEYRIGHT)  || key == KEYRIGHT) {
			key = KEYRIGHT;
			xer = 1;
			yer = 0;
		}
		
		field.array[pc.coords[points].y][pc.coords[points].x] = ' ';
		pc.coords[0].x += xer;
		pc.coords[0].y += yer;
		for (int i = 1; i <= points; i++) {
			if (pc.coords[0].x == pc.coords[i].x && pc.coords[0].y == pc.coords[i].y) { goto dead; }

			if (pc.coords[i].x == pc.coords[i-1].x) {
				xer = 0;
				if (pc.coords[i].y > pc.coords[i-1].y)
					yer = -1;
				else
					yer = 1;
			} else if (pc.coords[i].y == pc.coords[i-1].y) {
				yer = 0;
				if (pc.coords[i].x > pc.coords[i-1].x)
					xer = -1;
				else
					xer = 1;
			} else if (yer != 0) {
				yer = 0;
				if (pc.coords[i].x > pc.coords[i-1].x)
					xer = -1;
				else
					xer = 1;
			} else if (xer != 0) {
				xer = 0;
				if (pc.coords[i].y > pc.coords[i-1].y)
					yer = -1;
				else
					yer = 1;
			} else {
				system("cls");
				std::cout << "ERROR in position write\n";
				std::cout << "Press enter to close the application...";
				std::cin.get();
				goto endLoop;
			}
			pc.coords[i].x += xer;
			pc.coords[i].y += yer;
		}
		
		if (pc.coords[0].x == 0 || pc.coords[0].x == sizeS-1 || pc.coords[0].y == 0 || pc.coords[0].y == sizeS-1) { goto dead; }
		if (false) {
dead:
			system("cls");
			std::cout << "You died\n";
			std::cout << "Points: " << points << "\n";
			std::cout << "Press enter to close the application...";
			system("pause");
endLoop:
			break;
		}

		Sleep(delayS);
		system("cls");
	}

	return 0;
}