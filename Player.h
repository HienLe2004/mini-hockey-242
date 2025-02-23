#pragma once
#include "ECS.h"
#include "Components.h"
#include "SDL.h"
class Player {
private:
	Manager* manager;
	float originalSpeed = 3.0f;
	float speed = 3.0f;
	float dashedSpeed = 30.0f;
	float decreaseAmount = 1.0f;
	bool dashed = false;
	bool active = false;
	SDL_KeyCode dash;
public:
	Player(){}
	Entity* player;
	void setActive(bool b) { active = b; }
	bool isActive() { return active; }
	void init(Manager* m) {
		manager = m;
		auto& p(manager->addEntity());
		player = &p;
		p.addComponent<TransformComponent>(500, 300);
		p.addComponent<SpriteComponent>("images/Player.png", 128, 128, 64, 64);
		p.addComponent<PhysicComponent>();
		p.addComponent<KeyboardControlComponent>(SDLK_w, SDLK_d, SDLK_s, SDLK_a);
		p.addComponent<CircleColliderComponent>(Vector2D(0, 0), 32.0f);
	}
	void reset() {
		if (!dashed) 
			player->getComponent<KeyboardControlComponent>().reset();
			player->getComponent<PhysicComponent>().velocity = Vector2D(0,0);
	}
	void setDashKey(SDL_KeyCode k) { dash = k; }
	void handleEvents(SDL_Event event) {
		if (active) {
			Vector2D v = player->getComponent<KeyboardControlComponent>().handleEvents(event).Multiply(speed);
			player->getComponent<PhysicComponent>().velocity = v;
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == dash and !dashed) {
					speed = dashedSpeed;
					dashed = true;
				}
			}
		}
		//std::cout << player->getComponent<PhysicComponent>().velocity.x << "," << player->getComponent<PhysicComponent>().velocity.y << std::endl;
		if (dashed) {
			//std::cout << "dashed" << std::endl;
			if (speed > 3.0f) {
				speed -= decreaseAmount;
			}
			else {
				//std::cout << "reset" << std::endl;
				if (!active) {
					player->getComponent<KeyboardControlComponent>().reset();
					player->getComponent<PhysicComponent>().velocity = Vector2D(0, 0);
				}
				speed = originalSpeed;
				dashed = false;
			}
		}
	}
};