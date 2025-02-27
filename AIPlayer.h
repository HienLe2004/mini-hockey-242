#pragma once
#include "ECS.h"
#include "Components.h"
#include "SDL.h"
class AIPlayer {
private:
	Manager* manager;
	Entity* puck;
public:
	AIPlayer() {}
	Entity* player;
	void init(Manager* m, Entity* pu, Vector2D pos) {
		manager = m;
		puck = pu;
		auto& p(manager->addEntity());
		player = &p;
		p.addComponent<TransformComponent>(pos.x, pos.y);
		p.addComponent<SpriteComponent>("images/Player.png", 128, 128, 64, 64);
		p.addComponent<PhysicComponent>(5);
		p.addComponent<CircleColliderComponent>(Vector2D(0, 0), 32.0f);
		p.addComponent<AIComponent>(player, puck);
	}
	void reset() {
		player->getComponent<PhysicComponent>().velocity = Vector2D(0, 0);
	}
};