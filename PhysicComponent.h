#pragma once
#include "Components.h"
#include "Vector2D.h"
class PhysicComponent : public Component
{
public:
	TransformComponent* transform;
	Vector2D velocity;
	float decreasingRate = 0.1f;
	float mass = 1.0f;
	float maxSpeed = 10.0f;
	PhysicComponent() = default;
	PhysicComponent(float m) {
		mass = m;
	}
	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		velocity.x = 0.0f;
		velocity.y = 0.0f;
	}
	void update() override {
		if (velocity.Magnitude() > maxSpeed) {
			velocity.Normalize().Multiply(maxSpeed);
		}
		transform->position += velocity;
		velocity *= (1 - decreasingRate);
		//std::cout << transform->position.x << " " << transform->position.y << std::endl;
	}
};