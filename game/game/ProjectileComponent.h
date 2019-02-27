#pragma once

#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(int rng, int sp, Vector2D vel) : range(rng), speed(sp), velocity(vel)
	{}
	~ProjectileComponent() {}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
		//std::cout << transform->position << std::endl;
	}

	void update() override
	{
		distance += speed; //speed added to distance each frame

		//check to see how far travelled and if out of bounds
		if (distance > range)
		{
			std::cout << "Projectile out of range" << std::endl;
			entity->destroy();
		}
		else if (transform->position.x > Game::camera.x + Game::camera.w || //out of right hand side of screen
				transform->position.x < Game::camera.x || //out of left hand side
				transform->position.y > Game::camera.y + Game::camera.h || //out of bottom
				transform->position.y < Game::camera.y) //out of top
		{
			std::cout << "Projectile out of bounds" << std::endl;
			entity->destroy();
		}
	}

private:
	TransformComponent* transform;

	int range = 0;
	int speed = 0;
	int distance = 0;
	Vector2D velocity;
};