#include "GolfClub.h"
#include "deltaTime.h"

int GolfClub::init()
{
	proj_surface = IMG_Load("content/golf_clubs.png");
	if (proj_surface == NULL)
	{
		//if image could not be initalised throw error and stop program
		std::cerr << "could not initialize Golf UI!\n";
		std::cerr << SDL_GetError() << std::endl;
		return 1;
	}
	//set the texture from the image on the surface
	proj_texture = SDL_CreateTextureFromSurface(this->proj_renderer, proj_surface);
	gc_textureUI = SDL_CreateTextureFromSurface(this->proj_renderer, proj_surface);
	//for in game usages
	proj_positionSrc.x = 15;
	proj_positionSrc.y = 0;
	proj_positionSrc.w = clubWidth;
	proj_positionSrc.h = clubHight;

	//for UI drawing
	gc_positionUISrc.x = 0;
	gc_positionUISrc.y = 0;
	gc_positionUISrc.w = clubWidth;
	gc_positionUISrc.h = clubHight;

	SDL_FreeSurface(proj_surface);
	return 0;
}


void GolfClub::draw(SDL_Rect* camera)
{
	gc_destDraw = { proj_positionDest.x - camera->x, proj_positionDest.y - camera->y, proj_positionDest.w, proj_positionDest.h };
	//set position of the club
	if (collided)
	{
		proj_positionSrc.x = 0;
		SDL_RenderCopy(this->proj_renderer, this->proj_texture, &proj_positionSrc, &gc_destDraw);
	}
	else
	{
		proj_positionSrc.x = 15;
		proj_positionDest.x += (int)clubVelocityX;
		proj_positionDest.y += (int)clubVelocityY;
		proj_positionDest.w = clubWidth * 2;
		proj_positionDest.h = clubHight * 2;
		SDL_RenderCopy(this->proj_renderer, this->proj_texture, &proj_positionSrc, &gc_destDraw);
	}
	//check the velocity by using absolutes
	if (std::abs(clubVelocityX) < 1.0f && std::abs(clubVelocityY) < 1.0f)
	{
		clubVelocityX = 0;
		clubVelocityY = 0;
		collided = true;
	}
	const float decreaseRate = 2.0f;
	//X decrease in speed
	if (clubVelocityX > 0.0f)
		clubVelocityX -= decreaseRate * Time->GetDeltaTime();
	else if (clubVelocityX < 0.0f)
		clubVelocityX += decreaseRate * Time->GetDeltaTime();
	//Y decrease in speed
	if (clubVelocityY > 0.0f)
		clubVelocityY -= decreaseRate * Time->GetDeltaTime();
	else if (clubVelocityY < 0.0f)
		clubVelocityY += decreaseRate * Time->GetDeltaTime();
}

void GolfClub::drawAsUI()
{
	gc_positionUIDest.x = screenWidth - 80;
	gc_positionUIDest.y = 0;
	gc_positionUIDest.w = clubWidth * 4;
	gc_positionUIDest.h = clubHight * 4;
	if (isThrown)
	{
		gc_positionUISrc.x = 30;
		SDL_RenderCopy(this->proj_renderer, this->gc_textureUI, &gc_positionUISrc, &gc_positionUIDest);
	}
	if (!isThrown && !collided)
	{
		gc_positionUISrc.x = 0;
		SDL_RenderCopy(this->proj_renderer, this->gc_textureUI, &gc_positionUISrc, &gc_positionUIDest);
	}
}

	 //(this update is not needed on tick)
void GolfClub::update(bool used, int mouseX, int mouseY, int playerCenterX, int playerCenterY, SDL_Rect* camera)
{
	//set the variables on click once if isThrown is false (check calling method)
	proj_positionDest.x = playerCenterX;
	proj_positionDest.y = playerCenterY;
	isThrown = used;
	//cast to float for the atan2 method
	rotation = atan2(static_cast<float>(mouseY - (playerCenterY - camera->y)), static_cast<float>(mouseX - (playerCenterX - camera->x)));
	//truncating because we dotn need the decimal
	clubVelocityX = (speed * cos(rotation));
	clubVelocityY = (speed * sin(rotation));
}
