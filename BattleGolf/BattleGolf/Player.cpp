#include "Player.h"
#include "BallAmmo.h"
#include "GolfClub.h"
#include "PlayerUI.h"
#include "Enemy.h"

void Player::init()
{
	//load the surface with image in the content
	//use this area later to initalise the sprite sheet for animations
	p_surface = IMG_Load("content/player_sheet.png");
	if (p_surface == NULL)
	{
		//if image could not be initalised throw error and stop program
		std::cerr << "could not initialize picture!\n";
		std::cerr << SDL_GetError() << std::endl;
		return;
	}
	//set the texture from the image on the surface
	this->p_texture = SDL_CreateTextureFromSurface(this->p_renderer, p_surface);

	//setting the start location of the player sprite + use the image width and height
	p_positionSrc.x = 0;
	p_positionSrc.y = 0;
	p_positionSrc.w = playerWidth;
	p_positionSrc.h = playerHeight;

	//this is the destination rect that is used on the screen instead of the source
	p_positionDest.x = 400;
	p_positionDest.y = 800;
	p_positionDest.w = playerWidth * 2;
	p_positionDest.h = playerHeight * 2;

	//free the surface once the image is on screen
	SDL_FreeSurface(p_surface);

	for (int i = 0; i < ballCount; i++)
	{
		AmmoTotal.push_back(new BallAmmo(this->p_renderer, screenWidth, screenHeight, 1));
	}

	playerClub = new GolfClub(this->p_renderer, screenWidth, screenHeight, 5, Time);
	//create the UI for the player when it is initialised
	playerUI = new PlayerUI(this->p_renderer, p_camera, screenWidth, screenHeight, &stamina, &swings);
}

void Player::processInput(SDL_Event e)
{
	//method to handle the directional movement of the player
	directionalMovement();
}

void Player::directionalMovement()
{
	bool topCollision = p_positionDest.y > 0;
	bool bottomCollision = (p_positionDest.y + p_positionDest.h) < (screenHeight / 10) * mapHeight;
	bool rightCollison = (p_positionDest.x + p_positionDest.w) < (screenWidth / 10) * mapWidth;
	bool leftCollison = p_positionDest.x > 0;
	//move the character up the screen and set relevant animation frames
	if (movingUp && topCollision)
	{
		p_positionSrc.x += playerWidth + 4;
		if (p_positionSrc.x > 76)
		{
			p_positionSrc.x = 0;
		}
		p_positionSrc.y = (playerHeight * 3) + 12;
		p_positionDest.y -= speed;
	}
	//move the character down the screen and set relevant animation frames
	if (movingDown && bottomCollision)
	{
		p_positionSrc.x += playerWidth + 4;
		if (p_positionSrc.x > 76)
		{
			p_positionSrc.x = 0;
		}
		p_positionSrc.y = 0;
		p_positionDest.y += speed;
	}

	//move the character left across the screen and set relevant animation frames
	if (movingLeft && leftCollison)
	{
		p_positionSrc.x += playerWidth + 4;
		if (p_positionSrc.x > 76)
		{
			p_positionSrc.x = 0;
		}
		p_positionSrc.y = playerHeight + 4;
		p_positionDest.x -= speed;
	}

	//move the character right across the screen and set relevant animation frames
	if (movingRight && rightCollison)
	{
		p_positionSrc.x += playerWidth + 4;
		if (p_positionSrc.x > 76)
		{
			p_positionSrc.x = 0;
		}
		p_positionSrc.y = (playerHeight * 2) + 8;
		p_positionDest.x += speed;
	}
}


void Player::throwProjectile(const int& mouseX, const int& mouseY, const int& ammoType)
{
	if (ammoType == club)
	{
		if (hasThrownClub)
		{
			std::cerr << "No club to throw\n";
			return;
		}
		hasThrownClub = true;
		playerClub->update(hasThrownClub, mouseX, mouseY, p_positionDest.x, p_positionDest.y, p_camera);
	}
	else if(ammoType == ball)
	{
		if (ballCount == 0)
		{
			std::cerr << "No ball to throw\n";
			return;
		}
		for (auto aBall : AmmoTotal)
		{
			if (!aBall->used)
			{
				ballCount--;
				aBall->used = true;
				aBall->update(aBall->used, mouseX, mouseY, p_positionDest.x + (p_positionDest.w/2), p_positionDest.y + (p_positionDest.h / 2), p_camera);
				break;
			}
		}
	}
}

void Player::swingClub(const int& mouseX, const int& mouseY)
{
	swings++;
}

bool Player::update(std::vector<Enemy*> eList)
{
	if (canMove)
	{
		p_previousPos.x = p_positionDest.x;
		p_previousPos.y = p_positionDest.y;
		p_previousPos.w = p_positionDest.w;
		p_previousPos.h = p_positionDest.h;
	}
	for (auto& enemy : eList)
	{
		if (enemy->GetCollision())
		{
			enemyIntersect = true;
			if (enemyIntersect)
			{
				usingStamina = true;
				stamina-=4;
				break;
			}
		}
		else
		{
			enemyIntersect = false;
			usingStamina = false;
		}
	}
	switch (terrainCheck)
	{
	case SAND:
		speed = 12;
		canMove = true;
		usingStamina = true;
		if (stamina > 50)
		{
			stamina--;
		}
		break;
	case HIGHGRASS:
		speed = 18;
		canMove = true;
		usingStamina = true;
		if (stamina > 75)
		{
			stamina--;
		}
		break;
	case WATER:
		p_positionDest = p_previousPos;
		break;
	default:
		speed = 20;
		canMove = true;
		break;
	}
	//adjust stamina bar when flag is set or not set
	if (stamina < 100 && !usingStamina && !enemyIntersect) 
	{
		speed = 20;
		stamina += 5;
	}
	checkCollision(playerClub->GetClubPos());
	
	if (stamina <= 0)
	{
		return false;
	}
	return true;
}
void Player::draw()
{
	p_drawDest = { p_positionDest.x - p_camera->x, p_positionDest.y - p_camera->y, p_positionDest.w, p_positionDest.h };
	if (hasThrownClub)
	{
		playerClub->draw(p_camera);
	}
	for (auto& aBall : AmmoTotal)
	{
		if (aBall->used)
		{
			aBall->draw(p_camera);
		}
	}
	SDL_RenderCopy(this->p_renderer, this->p_texture, &p_positionSrc, &p_drawDest);
	drawUI();
}

void Player::drawUI()
{
	//Draw the player UI
	if (usingStamina || stamina < 100)
	{
		//dynamic UI elements 
		playerUI->draw(p_positionDest, playerWidth);
	}
	//static UI elements
	playerClub->drawAsUI();
	std::string ballsRemaining = "x " + std::to_string(ballCount);
	AmmoTotal[0]->drawAsUI(ballsRemaining);
	playerUI->update();
}

bool Player::checkCollision(SDL_Rect* other)
{
	if (playerClub->collided)
	{
		SDL_Rect check = { other->x - p_camera->x, other->y - p_camera->y, other->w, other->h };
		if (SDL_HasIntersection(&check, &p_drawDest))
		{
			//std::cout << "Player: pick up club" << std::endl;
			hasThrownClub = false;
			playerClub->isThrown = false;
			playerClub->collided = false;
			return true;
		}
	}
	return false;
}

SDL_Rect* Player::GetGolfClub()
{
	return playerClub->GetClubPos();
}

void Player::SetGolfClubState()
{
	playerClub->SetClubVelocity();
	swings += 5;
}

void Player::clean()
{
	//destroys the created texture when the player is destroyed
	SDL_DestroyTexture(this->p_texture);
	if (playerUI)
	{
		//clear UI from memory and set to NULL
		delete playerUI;
		playerUI = nullptr;
	}
	if (playerClub)
	{
		delete playerClub;
		playerClub = nullptr;
	}
	if (AmmoTotal.size() > 0)
	{
		for (auto& aBall : AmmoTotal)
		{
			if (aBall != NULL)
			{
				delete aBall;
				aBall = nullptr;
			}
		}
	}
	AmmoTotal.clear();
}
