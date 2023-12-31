#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <list>

class Particles
{
public:
	Particles(SDL_Renderer* Rend);
	virtual ~Particles();

	void Update();
	void Draw();
private:
	SDL_Surface* particleSurface;
	SDL_Renderer* ptc_Renderer;
	SDL_Texture* ptc_texture;
	std::list<SDL_Rect> particlePositions;

	Uint32 emissionsRate = 200;
	Uint32 emmisionsRange = 90;
	Uint32 Gravity = 2;

	Uint32 lastParticleSpawn = 0;
	Uint32 nextParticleSpawn = 0;

	SDL_Rect dest = { 0,0,0,0 };
};

