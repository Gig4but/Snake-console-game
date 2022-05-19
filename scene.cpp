/************************************************
 * Hadik, console game "Snake" for Windows
 *
 * Created by Oleg Petruny in 2022
 * like credit program for course Programing C++
 *
 * License MIT
 ************************************************/

#pragma warning(disable:6031)
#include "scene.h"
using namespace Scenes;

void Scene::change(std::shared_ptr<Scene> to) {
	Scene::_Get() = to;
	static GameEnvironment& Env = GameEnvironment::Get();
	Env.resetControls = true;
}

Home::Home() {
	static Controls& Input = Controls::Get();
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
			Scene::Get()->change(std::make_shared<Scenes::Game>());
			break;
		case 1:
		{
			std::vector<object> body, obstacles, food;
			Direction direction;
			unsigned int score;
			loadMessage = Saves::Load(body, obstacles, food, direction, score);
			if (loadMessage == "")
				Scene::Get()->change(std::make_shared<Scenes::Game>(body, obstacles, food, direction, score));
			break;
		}
		case 2:
			Scene::Get()->change(std::make_shared<Scenes::Options>());
			break;
		case 3:
			GameFunctions::Exit();
			break;
		default:
			Scene::Get()->change(std::make_shared<Scenes::Home>());
			break;
	}
}



Game::Game(bool createSnake) {
	static Controls& Input = Controls::Get();
	BIND(Controls::Key::Up, cursorUp);
	BIND(Controls::Key::Down, cursorDown);
	BIND(Controls::Key::Left, cursorLeft);
	BIND(Controls::Key::Right, cursorRight);
	BIND(Controls::Key::Esc, esc);

	if (createSnake)
		oleg = Snake(std::make_unique<deathInvoker>());
}

Game::Game(std::vector<object>& body, std::vector<object>& food, std::vector<object>& obstacles, Direction& direction, unsigned int score) : Game(false) {
	oleg = Snake(body, food, obstacles, direction, score, std::make_unique<deathInvoker>());
}

void Game::Draw(std::vector<std::vector<sixel>>& buffer) { oleg.Draw(buffer); }

void Game::cursorUp() { oleg.move(Direction::Up); }
void Game::cursorDown() { oleg.move(Direction::Down); }
void Game::cursorLeft() { oleg.move(Direction::Left); }
void Game::cursorRight() { oleg.move(Direction::Right); }
void Game::esc() { Saves::Save(oleg.ToString()); GameFunctions::Exit(); }
void Game::death() { Get()->change(std::make_shared<Scenes::death>(oleg.score)); }


death::death() {
	static Controls& Input = Controls::Get();
	BIND(Controls::Key::Up, cursorSwitch);
	BIND(Controls::Key::Down, cursorSwitch);
	_BIND(Controls::Key::Enter, enter, 1000);
}

void death::Draw(std::vector<std::vector<sixel>>& buffer) {
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

void death::cursorSwitch() {
	cursor = cursor == 0 ? 1 : 0;
}
void death::enter() {
	switch (cursor) {
		case 0:
			Scene::Get()->change(std::make_shared<Scenes::Home>());
			break;
		case 1:
			GameFunctions::Exit();
			break;
	}
}



Options::Options() {
	static Controls& Input = Controls::Get();
	BIND(Controls::Key::Up, cursorUp);
	BIND(Controls::Key::Down, cursorDown);
	BIND(Controls::Key::Left, cursorLeft);
	BIND(Controls::Key::Right, cursorRight);
	_BIND(Controls::Key::Enter, enter, 1000);
}

void Options::Draw(std::vector<std::vector<sixel>>& buffer) {
	static GameSettings& Sett = GameSettings::Get();
	std::string fps = Sett.fpscounter ? "true" : "false";
	const std::vector<std::string> buttons = {
		"World height:    <" + std::to_string(Sett.size.y) + ">",
		"World width:     <" + std::to_string(Sett.size.x) + ">",
		"Min. speed:      <" + std::to_string(Sett.startSpeed) + ">",
		"Obstacles limit: <" + std::to_string(Sett.obstaclesLimit) + ">",
		"Food limit:      <" + std::to_string(Sett.foodLimit) + ">",
		"Food rate:       <" + std::to_string(Sett.foodRate) + ">",
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
	static GameSettings& Sett = GameSettings::Get();
	static Console& Cmd = Console::Get();
	switch (cursor) {
		case 0:
			if (Sett.size.y > Sett.sizeRange.Min.y) {
				Sett.size.y--;
				if (Sett.foodLimit > (Sett.size.x * Sett.size.y)/2 - Sett.obstaclesLimit)
					Sett.foodLimit = (Sett.size.x * Sett.size.y)/2 - Sett.obstaclesLimit;
				if (Sett.obstaclesLimit > (Sett.size.x * Sett.size.y)/2 - Sett.foodLimit)
					Sett.obstaclesLimit = (Sett.size.x * Sett.size.y)/2 - Sett.foodLimit;
			}
			break;
		case 1:
			if (Sett.size.x > Sett.sizeRange.Min.x) {
				Sett.size.x--;
				if (Sett.foodLimit > (Sett.size.x * Sett.size.y)/2 - Sett.obstaclesLimit)
					Sett.foodLimit = (Sett.size.x * Sett.size.y)/2 - Sett.obstaclesLimit;
				if (Sett.obstaclesLimit > (Sett.size.x * Sett.size.y)/2 - Sett.foodLimit)
					Sett.obstaclesLimit = (Sett.size.x * Sett.size.y)/2 - Sett.foodLimit;
			}
			break;
		case 2:
			if (Sett.startSpeed > Sett.startSpeedRange.Min)
				Sett.startSpeed--;
			break;
		case 3:
			if (Sett.obstaclesLimit > Sett.obstaclesLimitRange.Min)
				Sett.obstaclesLimit--;
			break;
		case 4:
			if (Sett.foodLimit > Sett.foodLimitRange.Min)
				Sett.foodLimit--;
			break;
		case 5:
			if (Sett.foodRate > Sett.foodRateRange.Min)
				Sett.foodRate--;
			break;
		case 6:
			Sett.fpscounter = Sett.fpscounter ? false : true;
			Cmd.clear();
			break;
		default:
			break;
	}
}
void Options::cursorRight() {
	static GameSettings& Sett = GameSettings::Get();
	static Console& Cmd = Console::Get();
	switch (cursor) {
		case 0:
			if (Sett.size.y < Sett.sizeRange.Max.y)
				Sett.size.y++;
			break;
		case 1:
			if (Sett.size.x < Sett.sizeRange.Max.x)
				Sett.size.x++;
			break;
		case 2:
			if (Sett.startSpeed < Sett.startSpeedRange.Max)
				Sett.startSpeed++;
			break;
		case 3:
			if (Sett.obstaclesLimit < (Sett.size.x * Sett.size.y)/2 - Sett.foodLimit)
				Sett.obstaclesLimit++;
			break;
		case 4:
			if (Sett.foodLimit < (Sett.size.x * Sett.size.y)/2 - Sett.obstaclesLimit)
				Sett.foodLimit++;
			break;
		case 5:
			if (Sett.foodRate < Sett.foodRateRange.Max)
				Sett.foodRate++;
			break;
		case 6:
			Sett.fpscounter = Sett.fpscounter ? false : true;
			Cmd.clear();
			break;
		default:
			break;
	}
}
void Options::enter() {
	switch (cursor) {
		case 7:
			Scene::Get()->change(std::make_shared<Scenes::Home>());
			break;
		default:
			break;
	}
}
