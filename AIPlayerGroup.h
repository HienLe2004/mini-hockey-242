#pragma once
#include "AIPlayer.h"
#include "Vector2D.h"
class AIPlayerGroup {
public:
	AIPlayer* players[2];
	AIComponent* aiComponents[2];
	TransformComponent* puckTransform;
	AIPlayerGroup() {}
	~AIPlayerGroup() {
		for (int i = 0; i < 2; i++) {
			if (players[i] != nullptr) {
				delete players[i];
			}
		}
	}
	void init(Manager* m, Entity* puck) {
		puckTransform = &puck->getComponent<TransformComponent>();
		players[0] = new AIPlayer();
		players[0]->init(m, puck, Vector2D(900, 100));
		players[0]->player->getComponent<AIComponent>().baseX = 600;
		aiComponents[0] = &players[0]->player->getComponent<AIComponent>();
		players[1] = new AIPlayer();
		players[1]->init(m, puck, Vector2D(900, 500));
		players[1]->player->getComponent<AIComponent>().baseX = 950;
		aiComponents[1] = &players[1]->player->getComponent<AIComponent>();
	}
	void handleEvents(SDL_Event event) {
		if (aiComponents[0]->state == aiComponents[1]->state && 
			aiComponents[0]->state == DASH && aiComponents[0]->dashDirection == aiComponents[1]->dashDirection) {
			aiComponents[0]->setActive(true);
			aiComponents[1]->setActive(true);
		}
		else if (aiComponents[0]->moveDirection == aiComponents[1]->moveDirection &&
			aiComponents[0]->state != DASH && aiComponents[1]->state != DASH) {
			aiComponents[0]->setActive(true);
			aiComponents[1]->setActive(true);
		}
		else {
			//std::cout << "P1 " << aiComponents[0]->moveDirection.x << "," << aiComponents[0]->moveDirection.y << std::endl;
			//std::cout << "P2 " << aiComponents[1]->moveDirection.x << "," << aiComponents[1]->moveDirection.y << std::endl;
			if (aiComponents[0]->state != STAY && aiComponents[1]->state != STAY) {
				if (puckTransform->position.x < 650) {
					//std::cout << 1 << std::endl;
					aiComponents[0]->setActive(true);
					aiComponents[1]->setActive(false);
					players[1]->reset();	
				}
				else {
					//std::cout << 2 << std::endl;
					aiComponents[1]->setActive(true);
					aiComponents[0]->setActive(false);
					players[0]->reset();
				}
			}
			else if (aiComponents[0]->state == STAY) {
				//std::cout << 2 << std::endl;
				aiComponents[1]->setActive(true);
				aiComponents[0]->setActive(false);
				players[0]->reset();
			}
			else {
				//std::cout << 1 << std::endl;
				aiComponents[0]->setActive(true);
				aiComponents[1]->setActive(false);
				players[1]->reset();
			}
		}
		//std::cout << "Player 1 " << aiComponents[0]->isActive() << " " << aiComponents[0]->state << std::endl;
		//std::cout << "Player 2 " << aiComponents[1]->isActive() << " " << aiComponents[1]->state << std::endl;
	}
};