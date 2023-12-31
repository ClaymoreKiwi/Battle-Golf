#include "StateMachine.h"


StateMachine::StateMachine()
{
	init();
}

int StateMachine::init()
{
	//used to initialise the SDL Lib
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "could not initialize SDL2!\n";
		std::cerr << SDL_GetError() << std::endl;
		return 1;
	}

	//create a window to the screen with defined height and width
	window = SDL_CreateWindow("BattleGolf", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	//if window could not be initialised throw corresponding error
	if (window == NULL)
	{
		std::cerr << "could not initialize window!\n";
		std::cerr << SDL_GetError() << std::endl;
		return 1;
	}
	//window was successful - use it to initialise the renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//if renderer could not be initialised throw corresponding error
	if (renderer == NULL)
	{
		std::cerr << "could not initialize renderer!\n";
		std::cerr << SDL_GetError() << std::endl;
		return 1;
	}
	return 0;
}

void StateMachine::updateState()
{
	while (state != Exit)
	{
		switch (state)
		{
		case Menu:
		{
			#pragma region MainMenu loop
			MainMenu* mainMenu = new MainMenu(this->renderer, windowWidth, windowHeight);
			if (mainMenu->init() < 0)
				mainMenu->menuRunning();
			//menu loop
			while(mainMenu->menuRunning())
			{
				//run all updates
				mainMenu->update();
			}
			state = mainMenu->menuStateUpdate();
			//destroy menu
			delete mainMenu;
			mainMenu = nullptr;
			#pragma endregion
		}
			break;
		case GamePlay:
		{
			#pragma region Gameloop
			Gameloop* game = new Gameloop(this->renderer, windowWidth, windowHeight);
			//init added to start application and init all game aspects
			if (game->init() < 0)
				game->gameRunning();
				//game loop
			while (game->gameRunning())
			{
				//run all updates
				game->update();
			}
			state = game->gameStateUpdate();
			//destroy all things and free memory before closing
			delete game;
			game = nullptr;
			#pragma endregion
		}
			break;

		case Lose:
		{
			#pragma region Lose Game loop
			LoseScreen* loseScreen = new LoseScreen(this->renderer, windowWidth, windowHeight);
			if (loseScreen->init() < 0)
				loseScreen->LoseScreenRunning();
			//menu loop
			while (loseScreen->LoseScreenRunning())
			{
				//run all updates
				loseScreen->update();
			}
			state = loseScreen->LoseScreenStateUpdate();
			//destroy menu
			delete loseScreen;
			loseScreen = nullptr;
			#pragma endregion
		}
			break;
		case Win:
		{
			#pragma region Win Game loop
			WinScreen* winScreen = new WinScreen(this->renderer, windowWidth, windowHeight);
			if (winScreen->init() < 0)
				winScreen->WinScreenRunning();
			//menu loop
			while (winScreen->WinScreenRunning())
			{
				//run all updates
				winScreen->update();
			}
			state = winScreen->WinScreenStateUpdate();
			//destroy menu
			delete winScreen;
			#pragma endregion
		}
			break;
		case Exit:
		{
			SDL_DestroyWindow(window);
			SDL_Quit();
		}
			break;
		};
	}
}
