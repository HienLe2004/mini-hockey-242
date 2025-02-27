#pragma once
#include "Scene.h"
#include "Components.h"
#include "ECS.h"
#include "math.h"
#include "SDL.h"
#include "PlayerGroup.h"
#include "AIPlayerGroup.h"
#include "Puck.h"
class GameplayWithAIScene : public Scene {
private:
	bool isInitialized = false;
public:
	PlayerGroup* playerGroup;
	AIPlayerGroup* aiPlayerGroup;
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

		puck.init(m);
		puck.puck->getComponent<PhysicComponent>().decreasingRate = 0.005f;
		puck.puck->getComponent<TransformComponent>().position = Vector2D(200, 300);

		playerGroup = new PlayerGroup();
		playerGroup->init(m, SDLK_w, SDLK_d, SDLK_s, SDLK_a, SDLK_f, SDLK_g, SDLK_h);
		playerGroup->players[0]->player->getComponent<TransformComponent>().position = Vector2D(100, 100);
		playerGroup->players[1]->player->getComponent<TransformComponent>().position = Vector2D(100, 500);

		aiPlayerGroup = new AIPlayerGroup();
		aiPlayerGroup->init(m,puck.puck);
		aiPlayerGroup->players[0]->player->getComponent<TransformComponent>().position = Vector2D(900, 100);
		aiPlayerGroup->players[1]->player->getComponent<TransformComponent>().position = Vector2D(900, 500);



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
		playerGroup->handleEvents(event);
		aiPlayerGroup->handleEvents(event);
		puck.handleEvents(event);
	}
	void checkCollision() {
		if (!isInitialized) return;
		//Walls with players and puck
		for (int i = 0; i < 6; i++) {
			//Normal players
			for (int p = 0; p < 2; p++) {
				bool b = CheckCollision::CheckCollisionRectAndCircle(&walls[i]->getComponent<RectColliderComponent>(), &playerGroup->players[p]->player->getComponent<CircleColliderComponent>());
				if (b) {
					//std::cout << "Collided!" << std::endl;
					CheckCollision::ResolveCollistionRectAndCircle(&walls[i]->getComponent<RectColliderComponent>(),
						&playerGroup->players[p]->player->getComponent<CircleColliderComponent>(),
						&playerGroup->players[p]->player->getComponent<PhysicComponent>());
				}
			}
			//AI players
			for (int p = 0; p < 2; p++) {
				bool b = CheckCollision::CheckCollisionRectAndCircle(&walls[i]->getComponent<RectColliderComponent>(), &aiPlayerGroup->players[p]->player->getComponent<CircleColliderComponent>());
				if (b) {
					//std::cout << "Collided!" << std::endl;
					CheckCollision::ResolveCollistionRectAndCircle(&walls[i]->getComponent<RectColliderComponent>(),
						&aiPlayerGroup->players[p]->player->getComponent<CircleColliderComponent>(),
						&aiPlayerGroup->players[p]->player->getComponent<PhysicComponent>());
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
		for (int g = 0; g < 2; g++) {
			//Normal players
			for (int p = 0; p < 2; p++) {
				bool b = CheckCollision::CheckCollisionRectAndCircle(&invisibleLines[g*2]->getComponent<RectColliderComponent>(), &playerGroup->players[p]->player->getComponent<CircleColliderComponent>());
				if (b) {
					//std::cout << "Collided!" << std::endl;
					CheckCollision::ResolveCollistionRectAndCircle(&invisibleLines[g*2]->getComponent<RectColliderComponent>(),
						&playerGroup->players[p]->player->getComponent<CircleColliderComponent>(),
						&playerGroup->players[p]->player->getComponent<PhysicComponent>());
				}
			}
			//AI players
			for (int p = 0; p < 2; p++) {
				bool b = CheckCollision::CheckCollisionRectAndCircle(&invisibleLines[g*2+1]->getComponent<RectColliderComponent>(), &aiPlayerGroup->players[p]->player->getComponent<CircleColliderComponent>());
				if (b) {
					//std::cout << "Collided!" << std::endl;
					CheckCollision::ResolveCollistionRectAndCircle(&invisibleLines[g*2+1]->getComponent<RectColliderComponent>(),
						&aiPlayerGroup->players[p]->player->getComponent<CircleColliderComponent>(),
						&aiPlayerGroup->players[p]->player->getComponent<PhysicComponent>());
				}
			}
		}
		//Players with players
			//Players in the same group
		CheckCollision::ResolveCollisionCircleAndCircle(&playerGroup->players[0]->player->getComponent<CircleColliderComponent>(),
			&playerGroup->players[0]->player->getComponent<PhysicComponent>(),
			&playerGroup->players[1]->player->getComponent<CircleColliderComponent>(),
			&playerGroup->players[1]->player->getComponent<PhysicComponent>());
		CheckCollision::ResolveCollisionCircleAndCircle(&aiPlayerGroup->players[0]->player->getComponent<CircleColliderComponent>(),
			&aiPlayerGroup->players[0]->player->getComponent<PhysicComponent>(),
			&aiPlayerGroup->players[1]->player->getComponent<CircleColliderComponent>(),
			&aiPlayerGroup->players[1]->player->getComponent<PhysicComponent>());
			//Players in the other group
		for (int p1 = 0; p1 < 2; p1++) {
			for (int p2 = 0; p2 < 2; p2++) {
				CheckCollision::ResolveCollisionCircleAndCircle(&playerGroup->players[p1]->player->getComponent<CircleColliderComponent>(),
					&playerGroup->players[p1]->player->getComponent<PhysicComponent>(),
					&aiPlayerGroup->players[p2]->player->getComponent<CircleColliderComponent>(),
					&aiPlayerGroup->players[p2]->player->getComponent<PhysicComponent>());
			}
		}
		//Players with puck
		for (int p = 0; p < 2; p++) {
			CheckCollision::ResolveCollisionWithVelocityCircleAndCircle(&playerGroup->players[p]->player->getComponent<CircleColliderComponent>(),
				&playerGroup->players[p]->player->getComponent<PhysicComponent>(),
				&puck.puck->getComponent<CircleColliderComponent>(),
				&puck.puck->getComponent<PhysicComponent>());
		}
		for (int p = 0; p < 2; p++) {
			CheckCollision::ResolveCollisionWithVelocityCircleAndCircle(&aiPlayerGroup->players[p]->player->getComponent<CircleColliderComponent>(),
				&aiPlayerGroup->players[p]->player->getComponent<PhysicComponent>(),
				&puck.puck->getComponent<CircleColliderComponent>(),
				&puck.puck->getComponent<PhysicComponent>());
		}
	}
	void drawWalls(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);// Blue background
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); 
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