#pragma once
#include "Game.h"

class GameObj
{
public:
	GameObj(const char *textureSheet, int x, int y);
	~GameObj();

	void update();
	void render();

private:
	int xpos;
	int ypos;

	SDL_Texture *objTexture;
	SDL_Rect srcRect, destRect;
};

