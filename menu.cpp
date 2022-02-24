#include "menu.h"
using namespace Menus;

extern bool resetControls;
void Menu::change(std::shared_ptr<Menu*> to) {
	(*scene)->~Menu();
	scene = to;
	resetControls = true;
	(*scene)->AfterConstructor();
}

void Home::AfterConstructor() {
	BIND(Controls::Key::Up, cursorUp);
	BIND(Controls::Key::Down, cursorDown);
	_BIND(Controls::Key::Enter, enter, 1000);
}

void Home::Draw(std::vector<std::vector<sixel>>& buffer) {
	const int size = 4;
	const std::string buttons[size] = { "#New game#", "#Load game#", "#Options#", "#Exit#" };
	for (int i = 0; i < size; i++) {
		if (cursor == i) {
			buffer.push_back({ sixel(buttons[i], Color::BlackWhite) });
		} else {
			buffer.push_back({ sixel(buttons[i]) });
		}
	}
	if (loadMessage != "")
		buffer.push_back({ sixel("==="+loadMessage+"===")});
}

void Home::cursorUp() {
	if (cursor > 0)
		cursor--;
	else
		cursor = 3;
}
void Home::cursorDown() {
	if (cursor < 3)
		cursor++;
	else
		cursor = 0;
}
void Home::enter() {
	switch (cursor) {
		case 0:
			(*scene)->change(std::make_shared<Menu*>(new Menus::Game()));
			break;
		case 1:
			loadMessage = Saves::LoadGame();
			break;
		case 2:
			(*scene)->change(std::make_shared<Menu*>(new Menus::Options()));
			break;
		case 3:
			exit();
			break;
		default:
			(*scene)->change(std::make_shared<Menu*>(new Menus::Home()));
			break;
	}
}



void Game::AfterConstructor() {
	BIND(Controls::Key::Up, cursorUp);
	BIND(Controls::Key::Down, cursorDown);
	BIND(Controls::Key::Left, cursorLeft);
	BIND(Controls::Key::Right, cursorRight);
	BIND(Controls::Key::Esc, esc);

	if (score == -1) {
		oleg = Snake();
	} else {
		oleg = Snake(*body, *food, *obstacles, score, direction);
		score = -1;
	}
}

void Game::Draw(std::vector<std::vector<sixel>>& buffer) { oleg.Draw(buffer); }

void Game::cursorUp() { oleg.move(Direction::Up); }
void Game::cursorDown() { oleg.move(Direction::Down); }
void Game::cursorLeft() { oleg.move(Direction::Left); }
void Game::cursorRight() { oleg.move(Direction::Right); }
void Game::esc() { oleg.Save(); exit(); }



void Death::AfterConstructor() {
	BIND(Controls::Key::Up, cursorSwitch);
	BIND(Controls::Key::Down, cursorSwitch);
	BIND(Controls::Key::Enter, enter);
}

void Death::Draw(std::vector<std::vector<sixel>>& buffer) {
	buffer.push_back({ sixel("SCORE: " + std::to_string(score)) });
	const int size = 2;
	const std::string buttons[size] = { "#To menu#", "#Exit game#" };
	for (int i = 0; i < size; i++) {
		if (cursor == i) {
			buffer.push_back({ sixel(buttons[i], Color::BlackWhite) });
		} else {
			buffer.push_back({ sixel(buttons[i]) });
		}
	}
}

void Death::cursorSwitch() {
	cursor = cursor == 0 ? 1 : 0;
}
void Death::enter() {
	switch (cursor) {
		case 0:
			(*scene)->change(std::make_shared<Menu*>(new Menus::Home()));
			break;
		case 1:
			exit();
			break;
	}
}



void Options::AfterConstructor() {
	BIND(Controls::Key::Up, cursorUp);
	BIND(Controls::Key::Down, cursorDown);
	BIND(Controls::Key::Left, cursorLeft);
	BIND(Controls::Key::Right, cursorRight);
	_BIND(Controls::Key::Enter, enter, 1000);
}

void Options::Draw(std::vector<std::vector<sixel>>& buffer) {
	std::string fps = fpscounter ? "true" : "false";
	const std::vector<std::string> buttons = {
		"World height:    <" + std::to_string(size.y) + ">",
		"World width:     <" + std::to_string(size.x) + ">",
		"Min. speed:      <" + std::to_string(minspeed) + ">",
		"Obstacles limit: <" + std::to_string(obstaclesLimit) + ">",
		"Food limit:      <" + std::to_string(foodLimit) + ">",
		"Food rate:       <" + std::to_string(foodRate) + ">",
		"FPS Counter:     <" + fps + ">",
		"\n#BACK#"
	};
	for (int i = 0; i < buttons.size(); i++) {
		if (cursor == i) {
			buffer.push_back({ sixel(buttons[i], Color::BlackWhite) });
		} else {
			buffer.push_back({ sixel(buttons[i]) });
		}
	}
}

void Options::cursorUp() { 
	if (cursor > 0)
		cursor--;
	else
		cursor = 7;
}
void Options::cursorDown() { 
	if (cursor < 7)
		cursor++;
	else
		cursor = 0;
}
void Options::cursorLeft() { 
	switch (cursor) {
		case 0:
			if (size.y > 5) {
				size.y--;
				if (foodLimit > (size.x*size.y)/2 - obstaclesLimit)
					foodLimit = (size.x*size.y)/2 - obstaclesLimit;
				if (obstaclesLimit > (size.x*size.y)/2 - foodLimit)
					obstaclesLimit = (size.x*size.y)/2 - foodLimit;
			}
			break;
		case 1:
			if (size.x > 5) {
				size.x--;
				if (foodLimit > (size.x*size.y)/2 - obstaclesLimit)
					foodLimit = (size.x*size.y)/2 - obstaclesLimit;
				if (obstaclesLimit > (size.x*size.y)/2 - foodLimit)
					obstaclesLimit = (size.x*size.y)/2 - foodLimit;
			}
			break;
		case 2:
			if (minspeed > 2)
				minspeed--;
			break;
		case 3:
			if (obstaclesLimit > 0)
				obstaclesLimit--;
			break;
		case 4:
			if (foodLimit > 1)
				foodLimit--;
			break;
		case 5:
			if (foodRate > 1)
				foodRate--;
			break;
		case 6:
			fpscounter = fpscounter ? false : true;
			cmd.clear();
			break;
		default:
			break;
	}
}
void Options::cursorRight() { 
	switch (cursor) {
		case 0:
			if (size.y < 63)
				size.y++;
			break;
		case 1:
			if (size.x < 63)
				size.x++;
			break;
		case 2:
			if (minspeed < 20)
				minspeed++;
			break;
		case 3:
			if (obstaclesLimit < (size.x*size.y)/2 - foodLimit)
				obstaclesLimit++;
			break;
		case 4:
			if (foodLimit < (size.x*size.y)/2 - obstaclesLimit)
				foodLimit++;
			break;
		case 5:
			if (foodRate < 100)
				foodRate++;
			break;
		case 6:
			fpscounter = fpscounter ? false : true;
			cmd.clear();
			break;
		default:
			break;
	}
}
void Options::enter() {
	switch (cursor) {
		case 7:
			(*scene)->change(std::make_shared<Menu*>(new Menus::Home()));
			break;
		default:
			break;
	}
}