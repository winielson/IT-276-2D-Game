#include "GameObj.h"
#include "TextureManager.h"


GameObj::GameObj(const char *textureSheet, int x, int y)
{
	objTexture = TextureManager::LoadTexture(textureSheet);

	xpos = x;
	ypos = y;
}


GameObj::~GameObj()
{
}

void GameObj::update()
{
	xpos++;
	ypos++;

	srcRect.h = 32;
	srcRect.w = 32;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = srcRect.w *2;
	destRect.h = srcRect.w * 2;
}

void GameObj::render()
{
	SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect); 
}
