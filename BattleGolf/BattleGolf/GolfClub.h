#pragma once
#include "Projectile.h"

class deltaTime;

class GolfClub : public Projectile
{
public:
	GolfClub(SDL_Renderer* r, const int& screenW, const int& screenH, int&& scoreCost, deltaTime* Time)
		:Projectile(r, screenW, screenH, scoreCost), Time(Time)
	{
		init();
	}
	int init();
	void draw(SDL_Rect*);
	void drawAsUI();
	void update(bool thrown,int mouseX, int mouseY, int playerPosX, int playerPosY, SDL_Rect* camera);
	~GolfClub()
	{
		SDL_DestroyTexture(gc_textureUI);
	}
	SDL_Rect* GetClubPos()
	{
		return &proj_positionDest;
	}

	void SetClubVelocity()
	{
		clubVelocityX = 0;
		clubVelocityY = 0;
		collided = true;
	}

private:
	SDL_Texture*  gc_textureUI = nullptr;
	SDL_Rect	  gc_destDraw = {0,0,0,0};
	SDL_Rect	  gc_positionUIDest = { 0,0,0,0 }; //destination on that source to be 
	SDL_Rect	  gc_positionUISrc = { 0,0,0,0 }; //source rect for the projectile

	deltaTime* Time = nullptr;

	const int clubWidth = 15,
			  clubHight = 26;

	float clubVelocityX = 0,
		clubVelocityY = 0;

	float rotation = 0,
		  speed = 15;

public:
	bool  isThrown = false,
		  collided = false;
};

