#pragma once
#include "ECS.h"
#include "Components.h"
#include "SDL.h"
class Puck {
private:
	Manager* manager;
public:
	Puck() {}
	Entity* puck;
	void init(Manager* m) {
		manager = m;
		auto& p(manager->addEntity());
		puck = &p;
		p.addComponent<TransformComponent>(0, 0);
		p.addComponent<SpriteComponent>("images/puck.png", 128, 128, 32, 32);
		p.addComponent<PhysicComponent>();
		p.addComponent<CircleColliderComponent>(Vector2D(0, 0), 16.0f);
	}
	void handleEvents(SDL_Event event) {
		
	}
};