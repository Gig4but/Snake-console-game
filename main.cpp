/************************************************
 * Hadik, console game "Snake" for Windows
 *
 * Created by Oleg Petruny in 2022
 * like credit program for course Programing C++
 *
 * License MIT
 ************************************************/

#pragma warning(disable:4244)
#include <iostream>
#include <windows.h>
#include <memory>
#include <chrono>
#include <thread>
#include "globals.h"
#include "graphics.h"
#include "controls.h"
#include "scene.h"

using namespace std::chrono;
/* Main loop ************************************/
int main() {
	GameEnvironment& Env = GameEnvironment::Get();
	GameSettings& Sett = GameSettings::Get();

	Console& Cmd = Console::Get();
	Controls& Input = Controls::Get();

	Scene::change(std::make_shared<Scenes::Home>()); // initialize first screen
	while (Env.run) {
		/* stabilization ************************/
		Env.timer.second = Env.timer.first;
		Env.timer.first = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		Env.tickDelta = Env.timer.first - Env.timer.second;
		
		Env.tick++;
		Env.sec += Env.tickDelta;

		if (Env.tickDelta < Env.tickDelay)
			Sleep(Env.tickDelay - Env.tickDelta);

		if (Env.sec > 1000) {
			Env.tps = Env.tick;
			Env.tick = 0;
			Env.sec = 0;
		}

		/* graphics *****************************/
		Scene::Get()->Draw(Cmd.buffer);
		if (Sett.fpscounter)
			Cmd.buffer.push_back({ sixel("\nFPS: " + std::to_string(Env.tps)) });
			//Cmd.buffer.push_back({ sixel("\nEnv.second: " + std::to_string(((float)Env.sec)/1000) + "        ")});
		Cmd.update();

		/* input ********************************/
		if (Env.resetControls)
			Env.resetControls = false;
		else
			Input.processInput(Env.tickDelta, Env.resetControls);
	}

	return 0;
}
