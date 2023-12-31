#pragma once
#include "Projectile.h"
#include "FontRendering.h"

class BallAmmo : public Projectile
{
public:
	BallAmmo(SDL_Renderer* r, const int& screenW, const int& screenH, int&& scoreCost)
		:Projectile(r, screenW, screenH, scoreCost)
	{
		init();
	}
	int init();
	void draw(SDL_Rect*);
	void drawAsUI(std::string);
	bool CheckCollision(SDL_Rect& rect, Circle_Col& circle, int& health, int& speed);
	void update(bool used, int mouseX, int mouseY, int playerPosX, int playerPosY, SDL_Rect* camera);
	~BallAmmo()
	{
		SDL_DestroyTexture(gb_textureUI);
	}

	SDL_Rect* GetBulletPos()
	{
		return &proj_positionDest;
	}
	void SetBulletPos()
	{
		proj_positionDest.x = -25;
		proj_positionDest.y = -25;
		inCollision = false;
	}
	void SetBulletVelocity()
	{
		ballVelocityX = 0;
		ballVelocityY = 0;
	}
	Circle_Col& GetCollider()
	{
		return collider;
	}

private:
	SDL_Texture*  gb_textureUI = nullptr;
	SDL_Rect	  gb_destDraw = { 0,0,0,0 };
	SDL_Rect	  gb_positionUISrc = { 0,0,0,0 }; //source rect for the projectile
	SDL_Rect	  gb_positionUIDest = { 0,0,0,0 }; //destination on that source to be 
	std::unique_ptr<FontRendering> fontRenderer;
	Circle_Col collider;

	const int ballWidth = 25,
			  ballHeight = 25,
			  speed = 25;

	int	ballVelocityX = 0,
		ballVelocityY = 0;

	float rotation = 0;

public:
	bool used = false;
	bool inCollision = false;
};
