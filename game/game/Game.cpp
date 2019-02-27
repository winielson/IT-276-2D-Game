#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>

Map *map;
Manager manager;

SDL_Renderer *Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, 800, 640 };

AssetManager *Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

auto &player(manager.addEntity());
auto &label(manager.addEntity());

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char *title, int width, int height, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		printf("Subsytems Initialized!\n");

		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	}

	//initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("Error : SDL_ttf\n");
	}

	assets->AddTexture("terrain", "images/map2/dungfloors_ts.png");
	assets->AddTexture("player", "images/animations/player_animations.png");
	assets->AddTexture("projectile", "images/projectiles/test_proj.png");

	//font AM
	assets->AddFont("comic", "fonts/comic.ttf", 16);

	map = new Map("terrain", 3, 32);

	//ECS Implementation
	map->LoadMap("images/map2/map1.map", 25, 20);

	player.addComponent<TransformComponent>(4);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);

	//font ECS
	SDL_Color white = { 255, 255, 255, 255 };
	label.addComponent<UILabel>(10, 10, "Comic Sans BABY", "comic", white);

	assets->CreateProjectile(Vector2D(player.getComponent<TransformComponent>().position.x + 150, player.getComponent<TransformComponent>().position.y + 150), Vector2D(5, 0), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(100, 600), Vector2D(2, 0), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(500, 500), Vector2D(2, 0), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(300, 400), Vector2D(2, 0), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(200, 400), Vector2D(2, 0), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(300, 300), Vector2D(2, 0), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(400, 200), Vector2D(2, 0), 200, 2, "projectile");
}

// passes all the tiles in their respective groups
auto &tiles(manager.getGroup(Game::groupMap));
auto &colliders(manager.getGroup(Game::groupColliders));
auto &players(manager.getGroup(Game::groupPlayers));
auto &enemies(manager.getGroup(Game::groupEnemies));
auto &projectiles(manager.getGroup(Game::groupProjectiles));

void Game::handleEvents()
{
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::update()
{
	//temper variables
	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;

	//constructing string stream (variable that holds output string)
	std::stringstream ss;
	ss << "Player Position: " << playerPos;
	label.getComponent<UILabel>().SetLabelText(ss.str(), "comic");

	manager.refresh();
	manager.update();	

	for (auto &c : colliders)
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol)) //collision detected if true
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	//projectile collision
	for ( auto &pr : projectiles)
	{
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, pr->getComponent<ColliderComponent>().collider))
		{
			std::cout << "Projectile hit player" << std::endl;
			pr->destroy();
			//do stuff heredsadsad
		}
	}

	//camera based on player position
	camera.x = player.getComponent<TransformComponent>().position.x - 400;
	camera.y = player.getComponent<TransformComponent>().position.y - 320;

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;
}

void Game::render() //render objects to the screen
{
	SDL_RenderClear(renderer);

	for (auto & t : tiles) //draws each tile
	{
		t->draw();
	}

	for (auto &c : colliders) //draws colliders
	{
		c->draw();
	}

	for (auto & p : players) //draws each player
	{
		p->draw();
	}

	for (auto & pr : projectiles) //draws each projectile
	{
		pr->draw();
	}

	for (auto & e : enemies) //draws each enemy
	{
		e->draw();
	}

	label.draw();

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
