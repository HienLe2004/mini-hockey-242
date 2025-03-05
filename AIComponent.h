#pragma once
#include "Components.h"
#include "Vector2D.h"
#include "Puck.h"
enum AIState {MOVE,DASH,STAY};
class AIComponent : public Component
{
private:
	Entity* player;
	PhysicComponent* playerPhysic;
	Entity* puck;
	PhysicComponent* puckPhysic;
	float originalSpeed = 3.0f;
	float speed = 3.0f;
	float dashedSpeed = 10.0f;
	float decreaseAmount = 0.5f;
	bool dashed = false;
	bool active = true;
	Vector2D direction;
	Vector2D targetPoint;
public:
	AIState state = MOVE;
	Vector2D dashDirection;
	Vector2D moveDirection;
	float baseX = 600;	
	AIComponent(Entity* p, Entity* pu) {
		player = p;
		playerPhysic = &p->getComponent<PhysicComponent>();
		puck = pu;
		puckPhysic = &pu->getComponent<PhysicComponent>();
	}
	void init() override {
		direction = Vector2D(0, 0);
		dashDirection = Vector2D(0, 0);
		moveDirection = Vector2D(0, 0);
		targetPoint = player->getComponent<TransformComponent>().position;
	}
	bool isActive() { return active; }
	void setActive(bool b) { active = b; }
	void update() override {
		//Puck from front
		if (puckPhysic->transform->position.x < playerPhysic->transform->position.x) {
			//Catch puck
			targetPoint = Vector2D(baseX, puckPhysic->transform->position.y);
			//Hit puck
			if (puckPhysic->velocity.x >= 0 || puckPhysic->velocity.Magnitude() < 1) {
				if (puckPhysic->transform->position.x > playerPhysic->transform->position.x - 80 &&
					(puckPhysic->transform->position.y > playerPhysic->transform->position.y - 50 ||
						puckPhysic->transform->position.y < playerPhysic->transform->position.y + 50)) {
					float xDash = -1;
					float yDash = 0;
					if (puckPhysic->transform->position.y < playerPhysic->transform->position.y - 20) {
						yDash = -1;
					}
					else if (puckPhysic->transform->position.y > playerPhysic->transform->position.y + 20) {
						yDash = 1;
					}
					if (yDash == 0) {
						xDash = -1;
					}
					else {
						float tanDash = (playerPhysic->transform->position.x - puckPhysic->transform->position.x) / abs(playerPhysic->transform->position.y - puckPhysic->transform->position.y);
						if (tanDash < 0.8) {
							xDash = 0;
						}
					}
					dashDirection = Vector2D(xDash, yDash);
					//std::cout << "DASH " << xDash << "," << yDash << std::endl;
					//std::cout << puckPhysic->transform->position.x << "," << puckPhysic->transform->position.y << std::endl;
					//std::cout << playerPhysic->transform->position.x << "," << playerPhysic->transform->position.y << std::endl;
					state = DASH;
				}
			}
			//Move toward and hit slow puck shooted from front
			if (puckPhysic->transform->position.x > 500 && puckPhysic->velocity.Magnitude() < 1) {
				targetPoint = Vector2D(puckPhysic->transform->position.x, puckPhysic->transform->position.y);
				/*std::cout << "chase" << std::endl;*/
			}
		}
		//Puck from behind
		else if (puckPhysic->transform->position.x > playerPhysic->transform->position.x && puckPhysic->transform->position.x < 1000) {
			//Dodge puck
			if (puckPhysic->velocity.Magnitude() >= 0) {
				if (puckPhysic->transform->position.y < 100) {
					if (playerPhysic->transform->position.y > puckPhysic->transform->position.y + 100) {
						targetPoint = Vector2D(baseX, playerPhysic->transform->position.y);
					}
					else {
						targetPoint = Vector2D(baseX, puckPhysic->transform->position.y + 100);
					}
				}
				else if (puckPhysic->transform->position.y > 500) {
					if (playerPhysic->transform->position.y < puckPhysic->transform->position.y - 100) {
						targetPoint = Vector2D(baseX, playerPhysic->transform->position.y);
					}
					else {
						targetPoint = Vector2D(baseX, puckPhysic->transform->position.y - 100);
					}
				}
				else if (puckPhysic->transform->position.y >= playerPhysic->transform->position.y) {
					targetPoint = Vector2D(baseX, puckPhysic->transform->position.y - 100);
				}
				else if (puckPhysic->transform->position.y < playerPhysic->transform->position.y) {
					targetPoint = Vector2D(baseX, puckPhysic->transform->position.y + 100);
				}
			}
			//std::cout << "Target dodge " << puckPhysic->velocity.Magnitude() << " " << targetPoint.x << "," << targetPoint.y << std::endl;
			//else {
			//	//Move backward and hit slow puck shooted from behind
			//	targetPoint = Vector2D(puckPhysic->transform->position.x + 200, playerPhysic->transform->position.y);
			//}
		}
		else {
			targetPoint = Vector2D(baseX, 300);
		}
		//std::cout << targetPoint.x << "," << targetPoint.y << " " << state << std::endl;
		int xD = 0;
		int yD = 0;
		if (targetPoint.x > playerPhysic->transform->position.x + 2) {
			xD = 1;
		}
		else if (targetPoint.x < playerPhysic->transform->position.x - 2) {
			xD = -1;
		}
		if (targetPoint.y > playerPhysic->transform->position.y + 2) {
			yD = 1;
		}
		else if (targetPoint.y < playerPhysic->transform->position.y - 2) {
			yD = -1;
		}
		moveDirection = Vector2D(xD, yD);
		moveDirection.Normalize();
		if (state != DASH) {
			if (moveDirection.Magnitude() == 0) {
				state = STAY;
			}
			else {
				state = MOVE;
			}
		}
		//std::cout << moveDirection.x << "," << moveDirection.y << std::endl;
		if (active) {
			if (state != DASH) {
				Vector2D dir = moveDirection;
				playerPhysic->velocity = dir.Multiply(speed);
			}
			else if (state == DASH && !dashed) {
				dashed = true;
				speed = dashedSpeed;
				Vector2D dir = dashDirection;
				playerPhysic->velocity = dir.Normalize().Multiply(speed);
			}
			//std::cout << "Velocity " << playerPhysic->velocity.x << "," << playerPhysic->velocity.y << std::endl;
			//std::cout << "Target " << targetPoint.x << "," << targetPoint.y << std::endl;
			//std::cout << "Current " << playerPhysic->transform->position.x << "," << playerPhysic->transform->position.y << std::endl;
		}
		if (dashed) {
			if (speed > 3.0f) {
				speed -= decreaseAmount;
			}
			else {
				if (!active) {
					player->getComponent<PhysicComponent>().velocity = Vector2D(0, 0);
				}
				speed = originalSpeed;
				dashed = false;
				state = STAY;
			}
		}
	}
};