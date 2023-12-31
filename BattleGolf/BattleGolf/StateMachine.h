#pragma once
#include "MainMenu.h"
#include "Gameloop.h"
#include "LoseScreen.h"
#include "WinScreen.h"

class StateMachine
{
public:
	StateMachine();
	int init();
	void updateState();

private:
	enum state
	{
		Menu = 0,
		GamePlay = 1,
		Lose = 2,
		Win = 3,
		Exit = 5
	};

	int state = Menu;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	const int windowWidth = 800,
			  windowHeight = 800;
};

