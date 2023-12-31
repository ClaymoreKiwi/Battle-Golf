#include "GolfBall.h"
#include "Player.h"
#include <fstream>
#include <random>
#include "TileMap.h"

bool GolfBall::fileCreated = false;

void GolfBall::init()
{
	gBall_surface = IMG_Load("content/golfBall.png");
	if (gBall_surface == NULL)
	{
		//if image could not be initalised throw error and stop program
		std::cerr << "could not initialize Golf Ball!\n";
		std::cerr << SDL_GetError() << std::endl;
		return;
	}
	//set the texture from the image on the surface
	this->gBall_texture = SDL_CreateTextureFromSurface(this->gBall_renderer, gBall_surface);
	// Use a random device to seed the random number generator
	std::random_device rd;

	// Use the Mersenne Twister engine for better distribution
	std::mt19937 gen(rd());

	// Define the range for the distribution
	std::uniform_int_distribution<int> distribution(400, 1000);
	int randYPos = distribution(gen);
	//setting the start location of the player sprite + use the image width and height
	gBall_positionSrc.x = 0;
	gBall_positionSrc.y = 0;
	gBall_positionSrc.w = golfBallWidth;
	gBall_positionSrc.h = golfBallHeight;

	gBall_positionDest.x = 400;
	gBall_positionDest.y = randYPos;
	gBall_positionDest.w = golfBallWidth;
	gBall_positionDest.h = golfBallHeight;

	gBall_range.radius = rangeWidth;

	gBall_collider.radius = golfBallWidth;
}

bool GolfBall::Update()
{
	playerInRange = CheckCollision(player->p_positionDest, gBall_range);
	isScored = CheckCollision(gBall_collider, *flagLocation);
	if (isScored)
	{
		velocityX = 0;
		velocityY = 0;
		const char* fileName = "./content/data/data.bin";
		std::ofstream outFile(fileName, std::ios::out | std::ios::binary);

		if (outFile.is_open())
		{
			// Use the binary file stream to write data from GetSwings
			int swings = player->GetSwings();
			outFile.write(reinterpret_cast<const char*>(&swings), sizeof(int));
			// Close the file
			outFile.close();
			GolfBall::fileCreated = true;
			//std::cout << "Binary file created successfully." << std::endl;
			return false;
		}
		else
		{
			std::cerr << "Error creating the file." << std::endl;
		}
	}
	return true;
}

void GolfBall::Draw()
{
	gBall_destDraw = { gBall_positionDest.x - gBall_camera->x, gBall_positionDest.y - gBall_camera->y, gBall_positionDest.w, gBall_positionDest.h };

	gBall_positionDest.x += (int)velocityX;
	gBall_positionDest.y += (int)velocityY;
	gBall_positionDest.w = golfBallWidth;
	gBall_positionDest.h = golfBallHeight;
	SDL_RenderCopy(this->gBall_renderer, this->gBall_texture, &gBall_positionSrc, &gBall_destDraw);
	//center the collision bounds of actual collider
	gBall_collider.x = gBall_positionDest.x + (gBall_positionDest.w / 2);
	gBall_collider.y = gBall_positionDest.y + (gBall_positionDest.h / 2);
	//center collision bounds of range detection
	gBall_range.x = gBall_collider.x;
	gBall_range.y = gBall_collider.y;
	//use absolute to account for negatives
	if (std::abs(velocityX) < 3.0f && std::abs(velocityY) < 3.0f)
	{
		velocityX = 0;
		velocityY = 0;
		inMotion = false;
	}
	const float decreaseRate = 3.5f;
	//X decrease in speed
	if (velocityX > 0.0f)
		velocityX -= decreaseRate * Time->GetDeltaTime();
	else if (velocityX < 0.0f)
		velocityX += decreaseRate * Time->GetDeltaTime();
	//Y decrease in speed
	if (velocityY > 0.0f)
		velocityY -= decreaseRate * Time->GetDeltaTime();
	else if (velocityY < 0.0f)
		velocityY += decreaseRate * Time->GetDeltaTime();

}

void GolfBall::Clean()
{
	SDL_DestroyTexture(gBall_texture);
}

bool GolfBall::CheckCollision(SDL_Rect& rectangle, Circle_Col& circle)
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
	if (distanceSquared < circle.radius * circle.radius)
	{
		return true;
	}
	return false;
}

bool GolfBall::CheckCollision(Circle_Col& a, Circle_Col& b)
{
	int totalRadiusSquared = a.radius + b.radius;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

	int deltaX = a.x - b.x;
	int deltaY = a.y - b.y;
	int distanceSquared = deltaX * deltaX + deltaY * deltaY;

	if (distanceSquared < totalRadiusSquared)
	{
		return true;
	}
	return false;
}


void GolfBall::SetVelocityAndDirection(bool hit, int mouseX, int mouseY)
{
	// Set the variables on click once if isThrown is false (check calling method)
	inMotion = hit;

	// Calculate the distance between the cursor and the ball
	double distance = sqrt(pow(mouseX - (gBall_destDraw.x), 2) + pow(mouseY - (gBall_destDraw.y), 2));

	const float maxSpeed = 20.0f;
	const float scalingFactor = 0.1f; 

	// Calculate the scaled speed
	float scaledSpeed = (float)distance * scalingFactor;

	// Ensure the speed doesn't exceed the maximum speed
	float finalSpeed = std::min(scaledSpeed, maxSpeed);

	// Cast to float for the atan2 method
	rotation = atan2(static_cast<float>(mouseY - gBall_destDraw.y), static_cast<float>(mouseX - gBall_destDraw.y));

	// Calculate the velocity based on the scaled speed
	velocityX = -finalSpeed * cos(rotation);  // Negative X component for opposite direction
	velocityY = -finalSpeed * sin(rotation);  // Negative Y component for opposite direction
}
