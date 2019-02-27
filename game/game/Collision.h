#pragma once

#include "SDL.h"

class ColliderComponent;

class Collision
{
public:
	static bool AABB(const SDL_Rect &recA, const SDL_Rect &recB);// Axis-Aligned Bounding Box Collision Detection
	static bool AABB(const ColliderComponent &colA, const ColliderComponent &colB);
};

