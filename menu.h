#pragma once

#include <vector>
#include <string>
#include <memory>
#include "controls.h"
#include "graphic.h"
#include "snake.h"
#include "saves.h"

extern Controls input;
extern Console cmd;
class Menu {
public:
	void virtual Draw(std::vector<std::vector<sixel>>& buffer) = 0;
	Menu() {};
	~Menu() { input.removeAllBindgs(); cmd.clear(); }
	void virtual AfterConstructor() {};
	void change(std::shared_ptr<Menu*> to);
};
extern std::shared_ptr<Menu*> scene;

//Automaticaly create structure in class for function invoke
#define FUNC(MenuName, FuncName) \
	void FuncName(); \
	struct FuncName ## Invoker : public function { \
		void invoke() override { \
			dynamic_cast<Menus::MenuName*>(*scene)->FuncName(); \
		} \
	};

#define BIND(Key, FuncName) \
	input.bind(Key, new FuncName ## Invoker());

#define _BIND(Key, FuncName, Delay) \
	input.bind(Key, new FuncName ## Invoker(), Delay);

/*
*
* MENU CREATION *
* 
class YourMenu :Menu {
	// define private/public variables, functions
	...
	// Constructor called after full initialization of object
	// Used for sensetive(stupid) objects
	void AfterConstructor();
};

*
* FUNTION TO KEYBOARD BINDING
* 
1] add invoker struct in class definition
  (Simply use FUNC macro)

2] somewhere add function definition
  (Need to use same function name as in FUNC macro)

3] bind function to controls manager in !WARNING! AfterConstruct()
  (Simply use BIND macro)
*/

extern void exit();
extern vector2D size;
extern unsigned int minspeed;
extern unsigned int obstaclesLimit;
extern unsigned int foodLimit;
extern unsigned int foodRate;
extern bool fpscounter;
namespace Menus {
	class Home :public Menu {
		int cursor = 0;
		std::string loadMessage = "";
	public:
		Home() {}
		void AfterConstructor();
		void Draw(std::vector<std::vector<sixel>>& buffer) override;

		FUNC(Home, cursorUp);
		FUNC(Home, cursorDown);
		FUNC(Home, enter);
	};

	class Game :public Menu {
		Snake oleg;
		std::vector<object>* body = nullptr;
		std::vector<object>* food = nullptr;
		std::vector<object>* obstacles = nullptr; 
		Direction direction;
		unsigned int score = -1;
	public:
		Game() {}
		Game(std::vector<object>* body, std::vector<object>* food, std::vector<object>* obstacles, unsigned int score, Direction direction) :
			body(body), food(food), obstacles(obstacles), score(score), direction(direction) {}
		void AfterConstructor();
		void Draw(std::vector<std::vector<sixel>>& buffer) override;

		FUNC(Game, cursorUp);
		FUNC(Game, cursorDown);
		FUNC(Game, cursorLeft);
		FUNC(Game, cursorRight);
		FUNC(Game, esc);
	};

	class Death :public Menu {
		int cursor = 0;
		unsigned int score = 0;
	public:
		Death() {}
		Death(unsigned int score) : score(score) {}
		void AfterConstructor();
		void Draw(std::vector<std::vector<sixel>>& buffer) override;

		FUNC(Death, cursorSwitch);
		FUNC(Death, enter);
	};
	
	class Options :public Menu { 
		int cursor = 0;
	public:
		Options() {}
		void AfterConstructor();
		void Draw(std::vector<std::vector<sixel>>& buffer) override;

		FUNC(Options, cursorUp);
		FUNC(Options, cursorDown);
		FUNC(Options, cursorLeft);
		FUNC(Options, cursorRight);
		FUNC(Options, enter);
	};	
}