#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

class Background
{
public:
	Background(SDL_Renderer*, const int&, const int&);
	void init();
	void draw();
	void clean();
	~Background()
	{
		clean();
	}

private:
	SDL_Texture*  b_texture = nullptr; //background texture
	SDL_Renderer* b_renderer = nullptr; //rendering capable from this class
	SDL_Surface*  b_surface = nullptr;
	SDL_Rect	  b_positionDest = { 0,0,0,0 }; //destination on that source to be viewed
	SDL_Rect	  b_positionSrc = { 0,0,0,0 }; //source rect for the player

	const int backgroundWH = 25,
			  screenWidth = 0,
			  screenHeight = 0;

};

