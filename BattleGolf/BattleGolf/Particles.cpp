#include "Particles.h"

Particles::Particles(SDL_Renderer* rend)
	:ptc_Renderer(rend)
{
	particleSurface = IMG_Load("./content/particles/particle.png");
	ptc_texture = SDL_CreateTextureFromSurface(ptc_Renderer, particleSurface);
	SDL_FreeSurface(particleSurface);
	dest.w = 20;
	dest.h = 20;
}

Particles::~Particles()
{
	SDL_DestroyTexture(ptc_texture);
}

void Particles::Update()
{
	if (SDL_GetTicks() - lastParticleSpawn >= nextParticleSpawn)
	{
		lastParticleSpawn = SDL_GetTicks();
		nextParticleSpawn = emissionsRate + (rand() % emmisionsRange);
		dest.x = (rand() % 768);
		dest.y = 0;
		particlePositions.push_back(dest);
	}
	for (auto& particle : particlePositions)
	{
		particle.y += Gravity;
	}
	particlePositions.remove_if([](SDL_Rect r) {return r.x >= 768 || r.y >= 768; });
}

void Particles::Draw()
{
	for (auto& particle : particlePositions)
	{
		SDL_RenderCopy(ptc_Renderer, ptc_texture, NULL, &particle);
	}
}
