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
#include <string>
#include <memory>
#include "globals.h"
#include "controls.h"
#include "graphics.h"
#include "saves.h"
#include "snake.h"

 /**
  * @brief Represents class of game scene used for export level to string (Draw())
  * and manipulate with local objects via keyboard bindings.
  * Please work with his own static instance by Get().
  */
class Scene {
	/**
	 * @brief Returns reference to original static instance.
	 *
	 * @return std::shared_ptr<Scene>& to original static instance.
	 */
	static std::shared_ptr<Scene>& _Get() {
		static std::shared_ptr<Scene> scene;
		return scene;
	}

public:
	/**
	 * @brief Returns constant reference to original static instance.
	 * 
	 * @return const std::shared_ptr<Scene>& to original static instance.
	 */
	static const std::shared_ptr<Scene>& Get() { return _Get(); }

	/**
	 * @brief Changes actual scene to new one.
	 *
	 * @param to instance of new scene.
	 */
	static void change(std::shared_ptr<Scene> to);

	/**
	 * @brief Exports scene into passed buffer.
	 *
	 * @param buffer Array for scene export.
	 */
	void virtual Draw(std::vector<std::vector<sixel>>& buffer) {};

	/**
	 * @brief Default constructor. Always clears all keyboard bindings and console window.
	 */
	Scene() { Controls::Get().removeAllBindgs(); Console::Get().clear(); }
};

/**
 * @brief Automatically creates structure in scene for function invoke.
 * 
 * @param SceneName The scene where bind needed.
 * @param FuncName The function what going to be used.
 */
#define FUNC(SceneName, FuncName) \
	void FuncName(); \
	struct FuncName ## Invoker : public function { \
		void invoke() override { \
			dynamic_cast<Scenes::SceneName&>(*Get()).FuncName(); \
		} \
	};

/**
 * @brief Automatically creates instance of function invoke structure and bind it to keyboard key.
 * 
 * @param Key The keyboard key what need the bindinding.
 * @param FuncName The function what going to be used.
 */
#define BIND(Key, FuncName) \
	Input.bind(Key, std::make_shared<FuncName ## Invoker>());

 /**
  * @brief Automatically creates instance of function invoke structure and bind it to keyboard key 
  * with specified next press delay.
  *
  * @param Key The keyboard key what need the bindinding.
  * @param FuncName The function what going to be used.
  * @param Delay The delay needed.
  */
#define _BIND(Key, FuncName, Delay) \
	Input.bind(Key, std::make_shared<FuncName ## Invoker>(), Delay);

/* Scene creation *******************************
 * 
 * class YourScene :Scene {
 * 	// define private/public variables, functions
 * };
 *
 ************************************************/

/* Function to keyboard binding *****************
 * 
 * 1] add invoker struct in class definition
 *   (Simply use FUNC macro)
 * 
 * 2] somewhere add function definition
 *   (Need to use same function name as in FUNC macro)
 * 
 * 3] bind function to controls manager in YourScene constructor
 *   (Simply use BIND macro)
 * 
 ************************************************/

/**
 * @brief Represents namespace of custom overloaded scenes inheriting original Scene class.
 */
namespace Scenes {
	/**
	 * @brief Represents home menu.
	 */
	class Home :public Scene {
		int cursor = 0; //!< cursor vertical position
		std::string loadMessage = ""; //!< message from Saves::Load()
	public:
		/**
		 * @brief Default constructor.
		 */
		Home();

		/**
		 * @brief Exports scene into passed buffer.
		 *
		 * @param buffer Array for scene export.
		 */
		void Draw(std::vector<std::vector<sixel>>& buffer) override;


		
		/**
		 * @brief Moves menu button focus up by one.
		 */
		FUNC(Home, cursorUp);

		/**
		 * @brief Moves menu button focus down by one.
		 */
		FUNC(Home, cursorDown);

		/**
		 * @brief Presses button.
		 */
		FUNC(Home, enter);
	};

	/**
	 * @brief Represents playable game scene.
	 */
	class Game :public Scene {
		Snake oleg; //!< playable game instance (Oleg's legit electronic game)
	public:
		/**
		 * @brief Default scene constructor.
		 * 
		 * @param createSnake True if you want to reinitialize playable game instance
		 */
		Game(bool createSnake = true);

		/**
		 * @brief Calls default constructor with game instance initialization.
		 *
		 * @param body The array for snake body (loads only position of body pieces).
		 * @param food The array for food positions on map.
		 * @param delay The array for obstacles positions on map.
		 * @param direction The direction of snake movement.
		 * @param score The actual score of game.
		 */
		Game(std::vector<object>& body, std::vector<object>& food, std::vector<object>& obstacles, Direction& direction, unsigned int score);
		
		/**
		 * @brief Exports scene into passed buffer.
		 *
		 * @param buffer Array for scene export.
		 */
		void Draw(std::vector<std::vector<sixel>>& buffer) override;



		/**
		 * @brief Changes direction of snake to the up.
		 */
		FUNC(Game, cursorUp);

		/**
		 * @brief Changes direction of snake to the down.
		 */
		FUNC(Game, cursorDown);

		/**
		 * @brief Changes direction of snake to the left.
		 */
		FUNC(Game, cursorLeft);

		/**
		 * @brief Changes direction of snake to the right.
		 */
		FUNC(Game, cursorRight);

		/**
		 * @brief Saves and exits game.
		 */
		FUNC(Game, esc);

		/**
		 * @brief Creates Death scene.
		 */
		FUNC(Game, death);
	};

	/**
	 * @brief Represents game after death scene.
	 */
	class death :public Scene {
		int cursor = 0; //!< cursor horizontal position
		unsigned int score = 0; //!< last earned in game score
	public:
		/**
		 * @brief Default constructor, leave the score equal to 0.
		 */
		death();

		/**
		 * @brief Advanced constructor what changes the score.
		 */
		death(unsigned int score) : death() { this->score = score; }

		/**
		 * @brief Exports scene into passed buffer.
		 *
		 * @param buffer Array for scene export.
		 */
		void Draw(std::vector<std::vector<sixel>>& buffer) override;



		/**
		 * @brief Switches between buttons exit game and back to menu.
		 */
		FUNC(death, cursorSwitch);

		/**
		 * @brief Presses button.
		 */
		FUNC(death, enter);
	};
	

	/**
	 * @brief Represents game options menu.
	 */
	class Options :public Scene { 
		int cursor = 0; //!< cursor vertical position
	public:
		/**
		 * @brief Default constructor.
		 */
		Options();

		/**
		 * @brief Exports scene into passed buffer.
		 *
		 * @param buffer Array for scene export.
		 */
		void Draw(std::vector<std::vector<sixel>>& buffer) override;



		/**
		 * @brief Moves menu button focus up by one.
		 */
		FUNC(Options, cursorUp);

		/**
		 * @brief Moves menu button focus down by one.
		 */
		FUNC(Options, cursorDown);

		/**
		 * @brief Decreases value of option.
		 */
		FUNC(Options, cursorLeft);

		/**
		 * @brief Increases value of option.
		 */
		FUNC(Options, cursorRight);

		/**
		 * @brief Presses button.
		 */
		FUNC(Options, enter);
	};	
}
