#pragma once
#include "Scene.h"
#include "Components.h"
#include "ECS.h"
#include "math.h"
#include "SDL.h"
#include "PlayerGroup.h"
#include "Puck.h"
class GameplayScene : public Scene {
private:
	bool isInitialized = false;
public:
	PlayerGroup* playerGroups[2];
	Entity* invisibleLines[4];
	Entity* topWall;
	Entity* bottomWall;
	Entity* topLeftWall;
	Entity* bottomLeftWall;
	Entity* topRightWall;
	Entity* bottomRightWall;
	Entity* walls[6];
	Puck puck;
	void init(Manager* m) override {
		manager = m;
		playerGroups[0] = new PlayerGroup();
		playerGroups[0]->init(m, SDLK_w, SDLK_d, SDLK_s, SDLK_a, SDLK_f, SDLK_g, SDLK_h);
		playerGroups[0]->players[0]->player->getComponent<TransformComponent>().position = Vector2D(100, 100);
		playerGroups[0]->players[1]->player->getComponent<TransformComponent>().position = Vector2D(100, 500);

		playerGroups[1] = new PlayerGroup();
		playerGroups[1]->init(m, SDLK_UP, SDLK_RIGHT, SDLK_DOWN, SDLK_LEFT, SDLK_KP_0, SDLK_KP_1, SDLK_KP_2);
		playerGroups[1]->players[0]->player->getComponent<TransformComponent>().position = Vector2D(900, 100);
		playerGroups[1]->players[1]->player->getComponent<TransformComponent>().position = Vector2D(900, 500);



		puck.init(m);
		puck.puck->getComponent<PhysicComponent>().decreasingRate = 0.005f;
		puck.puck->getComponent<TransformComponent>().position = Vector2D(200, 300);

		auto& line1(manager->addEntity());
		invisibleLines[0] = &line1;
		line1.addComponent<TransformComponent>(535, 300);
		line1.addComponent<RectColliderComponent>(Vector2D(0, 0), Vector2D(10, 600));
		auto& line2(manager->addEntity());
		invisibleLines[1] = &line2;
		line2.addComponent<TransformComponent>(465, 300);
		line2.addComponent<RectColliderComponent>(Vector2D(0, 0), Vector2D(10, 600));
		auto& line3(manager->addEntity());
		invisibleLines[2] = &line3;
		line3.addComponent<TransformComponent>(0, 300);
		line3.addComponent<RectColliderComponent>(Vector2D(0, 0), Vector2D(10, 600));
		auto& line4(manager->addEntity());
		invisibleLines[3] = &line4;
		line4.addComponent<TransformComponent>(1000, 300);
		line4.addComponent<RectColliderComponent>(Vector2D(0, 0), Vector2D(10, 600));
		
		auto& tWall(manager->addEntity());
		topWall = &tWall;
		walls[0] = topWall;
		tWall.addComponent<TransformComponent>(500, 0);
		tWall.addComponent<RectColliderComponent>(Vector2D(0, 0), Vector2D(1000, 10));

		auto& bWall(manager->addEntity());
		bottomWall = &bWall;
		walls[1] = bottomWall;
		bWall.addComponent<TransformComponent>(500, 600);
		bWall.addComponent<RectColliderComponent>(Vector2D(0, 0), Vector2D(1000, 10));

		auto& tlWall(manager->addEntity());
		topLeftWall = &tlWall;
		walls[2] = topLeftWall;
		tlWall.addComponent<TransformComponent>(0, 100);
		tlWall.addComponent<RectColliderComponent>(Vector2D(0, 0), Vector2D(10, 200));

		auto& blWall(manager->addEntity());
		bottomLeftWall = &blWall;
		walls[3] = bottomLeftWall;
		blWall.addComponent<TransformComponent>(0, 500);
		blWall.addComponent<RectColliderComponent>(Vector2D(0, 0), Vector2D(10, 200));

		auto& trWall(manager->addEntity());
		topRightWall = &trWall;
		walls[4] = topRightWall;
		trWall.addComponent<TransformComponent>(1000, 100);
		trWall.addComponent<RectColliderComponent>(Vector2D(0, 0), Vector2D(10, 200));

		auto& brWall(manager->addEntity());
		bottomRightWall = &brWall;
		walls[5] = bottomRightWall;
		brWall.addComponent<TransformComponent>(1000, 500);
		brWall.addComponent<RectColliderComponent>(Vector2D(0, 0), Vector2D(10, 200));

		isInitialized = true;
	}
	void handleEvents(SDL_Event event) {
		if (!isInitialized) return;
		playerGroups[0]->handleEvents(event);
		playerGroups[1]->handleEvents(event);
		puck.handleEvents(event);
	}
	void checkCollision() {
		if (!isInitialized) return;
		//Walls with players and puck
		for (int i = 0; i < 6; i++) {
			for (int g = 0; g < 2; g++) {
				for (int p = 0; p < 2; p++) {
					bool b = CheckCollision::CheckCollisionRectAndCircle(&walls[i]->getComponent<RectColliderComponent>(), &playerGroups[g]->players[p]->player->getComponent<CircleColliderComponent>());
					if (b) {
						//std::cout << "Collided!" << std::endl;
						CheckCollision::ResolveCollistionRectAndCircle(&walls[i]->getComponent<RectColliderComponent>(),
							&playerGroups[g]->players[p]->player->getComponent<CircleColliderComponent>(),
							&playerGroups[g]->players[p]->player->getComponent<PhysicComponent>());
					}
				}
			}
			bool b = CheckCollision::CheckCollisionRectAndCircle(&walls[i]->getComponent<RectColliderComponent>(), &puck.puck->getComponent<CircleColliderComponent>());
			if (b) {
				//std::cout << "Collided!" << std::endl;
				CheckCollision::ResolveCollistionRectAndCircleWithVelocity(&walls[i]->getComponent<RectColliderComponent>(),
					&puck.puck->getComponent<CircleColliderComponent>(),
					&puck.puck->getComponent<PhysicComponent>());
			}
		}
		//Invisible lines with players
		for (int g = 0; g < 4; g++) {
			for (int p = 0; p < 2; p++) {
				bool b = CheckCollision::CheckCollisionRectAndCircle(&invisibleLines[g]->getComponent<RectColliderComponent>(), &playerGroups[g%2]->players[p]->player->getComponent<CircleColliderComponent>());
				if (b) {
					//std::cout << "Collided!" << std::endl;
					CheckCollision::ResolveCollistionRectAndCircle(&invisibleLines[g]->getComponent<RectColliderComponent>(),
						&playerGroups[g%2]->players[p]->player->getComponent<CircleColliderComponent>(),
						&playerGroups[g%2]->players[p]->player->getComponent<PhysicComponent>());
				}
			}
		}
		//Players with players
			//Players in the same group
		for (int g = 0; g < 2; g++) {
			CheckCollision::ResolveCollisionCircleAndCircle(&playerGroups[g]->players[0]->player->getComponent<CircleColliderComponent>(),
				&playerGroups[g]->players[0]->player->getComponent<PhysicComponent>(),
				&playerGroups[g]->players[1]->player->getComponent<CircleColliderComponent>(),
				&playerGroups[g]->players[1]->player->getComponent<PhysicComponent>());
		}
			//Players in the other group
		for (int p1 = 0; p1 < 2; p1++) {
			for (int p2 = 0; p2 < 2; p2++) {
				CheckCollision::ResolveCollisionCircleAndCircle(&playerGroups[0]->players[p1]->player->getComponent<CircleColliderComponent>(),
					&playerGroups[0]->players[p1]->player->getComponent<PhysicComponent>(),
					&playerGroups[1]->players[p2]->player->getComponent<CircleColliderComponent>(),
					&playerGroups[1]->players[p2]->player->getComponent<PhysicComponent>());
			}
		}
		//Players with puck
		for (int g = 0; g < 2; g++) {
			for (int p = 0; p < 2; p++) {
				CheckCollision::ResolveCollisionWithVelocityCircleAndCircle(&playerGroups[g]->players[p]->player->getComponent<CircleColliderComponent>(),
					&playerGroups[g]->players[p]->player->getComponent<PhysicComponent>(),
					&puck.puck->getComponent<CircleColliderComponent>(),
					&puck.puck->getComponent<PhysicComponent>());
			}
		}
	}
	void drawWalls(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); // Green background
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
		//Mid
		SDL_Rect rect;
		rect.w = 10;
		rect.h = 600;
		rect.x = 500 - rect.w / 2;
		rect.y = 300 - rect.h / 2;
		SDL_RenderFillRect(renderer, &rect);
		//Top
		rect.w = 1000;
		rect.h = 10;
		rect.x = 500 - rect.w / 2;
		rect.y = 0 - rect.h / 2;
		SDL_RenderFillRect(renderer, &rect);
		//Bottom
		rect.w = 1000;
		rect.h = 10;
		rect.x = 500 - rect.w / 2;
		rect.y = 600 - rect.h / 2;
		SDL_RenderFillRect(renderer, &rect);
		//TopLeft
		rect.w = 10;
		rect.h = 200;
		rect.x = 0 - rect.w / 2;
		rect.y = 100 - rect.h / 2;
		SDL_RenderFillRect(renderer, &rect);
		//BottomLeft
		rect.w = 10;
		rect.h = 200;
		rect.x = 0 - rect.w / 2;
		rect.y = 500 - rect.h / 2;
		SDL_RenderFillRect(renderer, &rect);
		//TopRight
		rect.w = 10;
		rect.h = 200;
		rect.x = 1000 - rect.w / 2;
		rect.y = 100 - rect.h / 2;
		SDL_RenderFillRect(renderer, &rect);
		//BottomRight
		rect.w = 10;
		rect.h = 200;
		rect.x = 1000 - rect.w / 2;
		rect.y = 500 - rect.h / 2;
		SDL_RenderFillRect(renderer, &rect);
	}
};