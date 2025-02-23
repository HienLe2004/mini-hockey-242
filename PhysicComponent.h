#pragma once
#include "Components.h"
#include "Vector2D.h"
class PhysicComponent : public Component
{
private:
	TransformComponent* transform;
public:
	Vector2D velocity;
	float decreasingRate = 0.0f;
	PhysicComponent() = default;
	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		velocity.x = 0.0f;
		velocity.y = 0.0f;
	}
	void update() override {
		transform->position += velocity;
		velocity *= (1 - decreasingRate);
		//std::cout << transform->position.x << " " << transform->position.y << std::endl;
	}
};