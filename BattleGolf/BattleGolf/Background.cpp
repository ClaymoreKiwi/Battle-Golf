#include "Background.h"
#include "StateMachine.h"

Background::Background(SDL_Renderer* r, const int &screenW, const int &screenH)
	:b_renderer(r), screenWidth(screenW), screenHeight(screenH)
{
	init();
}

void Background::init()
{
	b_surface = IMG_Load("content/background/background_sheet.png");
	if (b_surface == NULL)
	{
		//if image could not be initalised throw error and stop program
		std::cerr << "could not initialize Background picture!\n";
		std::cerr << SDL_GetError() << std::endl;
		return;
	}
	//set the texture from the image on the surface
	this->b_texture = SDL_CreateTextureFromSurface(this->b_renderer, b_surface);

	//setting the start location of the player sprite + use the image width and height
	b_positionSrc.x = 0;
	b_positionSrc.y = 0;
	b_positionSrc.w = backgroundWH;
	b_positionSrc.h = backgroundWH;

	//this is the destination rect that is used on the screen instead of the source
	b_positionDest.x = 0;
	b_positionDest.y = 0;
	b_positionDest.w = backgroundWH;
	b_positionDest.h = backgroundWH;
	
	SDL_FreeSurface(b_surface);

}
void Background::draw()
{
	//draw tiles based on the size of the image (background images are 25x25)
	Uint8 offset = 25;
	//working through the rows
	for (b_positionDest.x = 0; b_positionDest.x < screenWidth ; b_positionDest.x += offset)
	{
		//working through the columns
		for (b_positionDest.y = 0; b_positionDest.y < screenHeight; b_positionDest.y += offset)
		{
			//establish even positions for X and Y positions
			bool isXEven = (b_positionDest.x / offset) % 2 == 0; 
			bool isYEven = (b_positionDest.y / offset) % 2 == 0;

			//determine the position of the correct tile 
			if ((isXEven && isYEven) || (!isXEven && !isYEven))
			{
				//render the normal tile
				b_positionSrc.x = 0;
				SDL_RenderCopy(this->b_renderer, this->b_texture, &b_positionSrc, &b_positionDest);
			}
			else
			{
				//render the offset tile
				b_positionSrc.x = offset;
				SDL_RenderCopy(this->b_renderer, this->b_texture, &b_positionSrc, &b_positionDest);
			}
		}
	}
}

void Background::clean()
{
	SDL_DestroyTexture(this->b_texture);
}