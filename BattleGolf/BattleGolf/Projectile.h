#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "Player.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

class Projectile
{
protected:
	Projectile(SDL_Renderer* r, const int& screenW, const int& screenH, int scoreCost)
		:proj_renderer(r), screenWidth(screenW), screenHeight(screenH), scoreCost(scoreCost)
	{}
	virtual int init() = 0;
	virtual void draw(SDL_Rect*) = 0;
	virtual void update();
	virtual ~Projectile() 
	{
		SDL_DestroyTexture(proj_texture);
	}

protected:
	SDL_Texture*  proj_texture = nullptr; //projectile sprite
	SDL_Renderer* proj_renderer = nullptr; //rendering capable from this class
	SDL_Rect	  proj_positionSrc = { 0,0,0,0 }; //source rect for the projectile
	SDL_Rect	  proj_positionDest = { 0,0,0,0 }; //destination on that source to be 
	SDL_Surface*  proj_surface = nullptr;

	int scoreCost = 0;

	const int screenWidth = 0,
		      screenHeight = 0;
};

