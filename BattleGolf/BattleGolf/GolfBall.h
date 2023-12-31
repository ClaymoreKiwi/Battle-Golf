#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "deltaTime.h"

class Player;
class TileMap;

struct Circle_Col
{
	int x, y;
	int radius;
};

class GolfBall
{
public:
	GolfBall(SDL_Renderer* r, const int& screenW, const int& screenH, SDL_Rect* camera, Player* player, Circle_Col* flag, deltaTime* Time, std::shared_ptr<TileMap> tm)
		:gBall_renderer(r), screenWidth(screenW), screenHeight(screenH), gBall_camera(camera), player(player), flagLocation(flag), Time(Time), tileMap(tm)
	{
		init();
	}
	void init();
	bool Update();
	void Draw();
	void Clean();
	bool CheckCollision(SDL_Rect& rect, Circle_Col& circle);
	bool CheckCollision(Circle_Col& circle1, Circle_Col& circle2);
	void SetVelocityAndDirection(bool used, int mouseX, int mouseY);

	inline bool GetInRangeOfPlayer()
	{
		return playerInRange;
	}
	inline bool GetMoition()
	{
		return inMotion;
	}
	~GolfBall()
	{
		Clean();
	}

private:
	SDL_Texture*  gBall_texture = nullptr; //Golf Ball sprite
	SDL_Renderer* gBall_renderer = nullptr; //rendering capable from this class
	SDL_Surface*  gBall_surface = nullptr; // surface to bring hte image in and add it as a texture to the rect
	SDL_Rect*	  gBall_camera = nullptr; //camera refrence from gameloop
	SDL_Rect	  gBall_positionSrc = { 0,0,0,0 };
	SDL_Rect	  gBall_positionDest = { 0,0,0,0 };
	SDL_Rect	  gBall_destDraw = { 0,0,0,0 };

	Circle_Col gBall_collider;
	Circle_Col gBall_range;
	Circle_Col* flagLocation;

	deltaTime* Time = nullptr;

	Player*	   player;

	std::shared_ptr<TileMap> tileMap;

	const int golfBallWidth = 25,
			  golfBallHeight = 25,
			  rangeWidth = 50,
			  rangeHeight = 50,
			  screenWidth = 0,
			  screenHeight = 0;

	float rotation = 0,
		  speed = 30,
		  velocityX = 0,
		  velocityY = 0;

	bool playerInRange = false,
		 isScored = false,
		 isOutOfBounds = false,
		 inMotion = false;

	static bool fileCreated;
};

