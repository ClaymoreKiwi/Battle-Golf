#include "BallAmmo.h"
#include "Enemy.h"

int BallAmmo::init()
{
	proj_surface = IMG_Load("content/golfball_ammo.png");
	if (proj_surface == NULL)
	{
		//if image could not be initalised throw error and stop program
		std::cerr << "could not initialize Golfball UI!\n";
		std::cerr << SDL_GetError() << std::endl;
		return 1;
	}
	//set the texture from the image on the surface
	proj_texture = SDL_CreateTextureFromSurface(this->proj_renderer, proj_surface);
	gb_textureUI = SDL_CreateTextureFromSurface(this->proj_renderer, proj_surface);
	//for in game drawing
	proj_positionSrc.x = 0;
	proj_positionSrc.y = 0;
	proj_positionSrc.w = ballWidth;
	proj_positionSrc.h = ballHeight;
	proj_positionDest.x = -1000;
	proj_positionDest.w = ballWidth;
	proj_positionDest.h = ballHeight;

	//for UI drawing
	gb_positionUISrc.x = 0;
	gb_positionUISrc.y = 0;
	gb_positionUISrc.w = ballWidth ;
	gb_positionUISrc.h = ballHeight;
	gb_positionUIDest.w = ballWidth * 2;
	gb_positionUIDest.h = ballHeight * 2;

	SDL_FreeSurface(proj_surface);
	//set radius of the bullet for collisions
	collider.radius = ballWidth / 2;

	fontRenderer = std::unique_ptr<FontRendering>(new FontRendering(proj_renderer, proj_positionDest.x, proj_positionDest.y));
    return 0;
}

void BallAmmo::draw(SDL_Rect* camera)
{
	if (!inCollision)
	{
		proj_positionDest.x += ballVelocityX;
		proj_positionDest.y += ballVelocityY;
		gb_destDraw = { proj_positionDest.x - camera->x, proj_positionDest.y - camera->y, proj_positionDest.w, proj_positionDest.h };
		SDL_RenderCopy(this->proj_renderer, this->proj_texture, &proj_positionSrc, &gb_destDraw);
		collider.x = gb_destDraw.x + (gb_destDraw.w / 2);
		collider.y = gb_destDraw.y + (gb_destDraw.h / 2);
	}
	if (inCollision)
	{
		proj_positionDest.x = -25;
		proj_positionDest.y = -25;
		gb_destDraw = { proj_positionDest.x - camera->x, proj_positionDest.y - camera->y, proj_positionDest.w, proj_positionDest.h };
		SDL_RenderCopy(this->proj_renderer, this->proj_texture, &proj_positionSrc, &gb_destDraw);
	}
}

void BallAmmo::drawAsUI(std::string formatText)
{
	int White[4] = { 255,245,217,255 }; //specified white for the font
	fontRenderer->draw(formatText, 100, 32, 60, 50, White);

	gb_positionUIDest.x = 10;
	gb_positionUIDest.y = 10;
	SDL_RenderCopy(this->proj_renderer, this->gb_textureUI, &gb_positionUISrc, &gb_positionUIDest);

}

bool BallAmmo::CheckCollision(SDL_Rect& rectangle, Circle_Col& circle, int& e_health, int& e_speed)
{

	// Find closest point on the rectangle to the circle's center
	int closestPointX, closestPointY;

	if (circle.x < rectangle.x)
	{
		closestPointX = rectangle.x;
	}
	else if (circle.x > rectangle.x + rectangle.w)
	{
		closestPointX = rectangle.x + rectangle.w;
	}
	else
	{
		closestPointX = circle.x;
	}

	if (circle.y < rectangle.y)
	{
		closestPointY = rectangle.y;
	}
	else if (circle.y > rectangle.y + rectangle.h)
	{
		closestPointY = rectangle.y + rectangle.h;
	}
	else
	{
		closestPointY = circle.y;
	}

	// Calculate the distance between the closest point and the circle's center
	int deltaX = circle.x - closestPointX;
	int deltaY = circle.y - closestPointY;
	int distanceSquared = deltaX * deltaX + deltaY * deltaY;

	// Check for collision
	if (distanceSquared < circle.radius * circle.radius && !inCollision)
	{
		e_health -= 1;
		e_speed -= 2;
		inCollision = true;
		return true;
	}

	return false;
}

void BallAmmo::update(bool used, int mouseX, int mouseY, int playerPosX, int playerPosY, SDL_Rect* camera)
{
	//set the variables on click once if isThrown is false (check calling method)
	proj_positionDest.x = playerPosX;
	proj_positionDest.y = playerPosY;
	used = used;
	//cast to float for the atan2 method
	rotation = atan2(static_cast<float>(mouseY - (playerPosY - camera->y)), static_cast<float>(mouseX - (playerPosX - camera->x)));
	//truncating because we dotn need the decimal
	ballVelocityX = static_cast<int>(speed * cos(rotation));
	ballVelocityY = static_cast<int>(speed * sin(rotation));
}
