#pragma once
#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "deltaTime.h"
#include "AudioPlayer.h"
#include "TileMap.h"
#include "MainMenu.h"
#include "GolfBall.h"

class Player;
class Enemy;

class Gameloop
{	
public:
	Gameloop(SDL_Renderer*, const int, const int);
	int init();
	std::string LoadMap(const int);
	void update();
	void draw();
	bool processInput();
	void CameraUpdate();
	bool gameRunning();
	int gameStateUpdate();
	void clean();
	~Gameloop()
	{
		clean();
	}

private:
	//for creating the window and the renderer for whatever is on screen
	SDL_Renderer* g_renderer = nullptr;
	AudioPlayer*  g_audioPlayer = nullptr;
	Player*	      player = nullptr;
	GolfBall*	  golfBall = nullptr;

	deltaTime* Time = nullptr;
	std::shared_ptr<TileMap> g_tiledMap;

	std::vector<Enemy*> enemyList;
	//for the window width and height to be accessed elsewhere
	const int windowWidth,
			  windowHeight;

		  int g_state = 0,
			  g_mapComplete = 0;

		  bool g_usingClub = true;

	enum MapNumber
	{
		MapOne,
		MapTwo,
		TotalMaps,
	};

public:
	SDL_Rect camera = {0,0, windowWidth, windowHeight}; // camera set position 0/0 and size of the screen

	struct //struct for the mouse position
	{
		int x = 0,
			y = 0;
	}mousePos;
};

