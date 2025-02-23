#pragma once
#include "Player.h"
#include "Vector2D.h"
class PlayerGroup {
private:
	SDL_KeyCode group, single;
public:
	Player* players[2];
	PlayerGroup(){}
	~PlayerGroup(){}
	void init(Manager* m, SDL_KeyCode up, SDL_KeyCode right, SDL_KeyCode down, SDL_KeyCode left, SDL_KeyCode dash, SDL_KeyCode group, SDL_KeyCode single) {
		this->group = group;
		this->single = single;
		for (int i = 0; i < 2; i++) {
			players[i] = new Player();
			players[i]->init(m);
			players[i]->player->getComponent<KeyboardControlComponent>().setKeys(up, right, down, left);
			players[i]->setDashKey(dash);
			players[i]->setActive(true);
		}
	}
	void handleEvents(SDL_Event event) {
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == group) {
				players[0]->setActive(true);
				players[1]->setActive(true);
			}
			else if (event.key.keysym.sym == single) {
				if (players[1]->isActive()) {
					players[0]->setActive(true);
					players[1]->setActive(false);
					players[1]->reset();
				}
				else {
					players[0]->setActive(false);
					players[0]->reset();
					players[1]->setActive(true);
				}
			}
		}
		players[0]->handleEvents(event);
		players[1]->handleEvents(event);
	}
};