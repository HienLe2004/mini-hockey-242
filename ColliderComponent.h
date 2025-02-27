#pragma once
#include "Components.h"
#include "Vector2D.h"
class CheckCollision;
class CircleColliderComponent : public Component
{
public:
	TransformComponent* transform;
	Vector2D origin;
	float radius;
	CircleColliderComponent() = default;
	CircleColliderComponent(Vector2D o, float r) {
		origin = o;
		radius = r;
	}
	Vector2D getWorldOrigin() {
		Vector2D temp = transform->position;
		return temp.Add(origin);
	}
	void init() override {
		transform = &entity->getComponent<TransformComponent>();
	}
};

class RectColliderComponent : public Component
{
public:
	TransformComponent* transform;
	Vector2D origin;
	Vector2D size;
	RectColliderComponent() = default;
	RectColliderComponent(Vector2D o, Vector2D s) {
		origin = o;
		size = s;
	}
	Vector2D getWorldOrigin() {
		Vector2D temp = transform->position;
		return temp.Add(origin);
	}
	void init() override {
		transform = &entity->getComponent<TransformComponent>();
	}
};
class CheckCollision {
public:
	static bool CheckCollisionRectAndCircle(RectColliderComponent* rect, CircleColliderComponent* circle) {
		Vector2D circleOrigin = circle->getWorldOrigin();
		Vector2D rectOrigin = rect->getWorldOrigin();
		float closestX = fmax(rectOrigin.x - rect->size.x / 2, fmin(circleOrigin.x, rectOrigin.x + rect->size.x / 2));
		float closestY = fmax(rectOrigin.y - rect->size.y / 2, fmin(circleOrigin.y, rectOrigin.y + rect->size.y / 2));

		float dx = circleOrigin.x - closestX;
		float dy = circleOrigin.y - closestY;
		float distanceSquared = dx * dx + dy * dy;

		return distanceSquared <= circle->radius * circle->radius;
	}
	static bool CheckCollisionCircleAndCircle(CircleColliderComponent* circle1, CircleColliderComponent* circle2) {
		Vector2D origin1 = circle1->getWorldOrigin();
		Vector2D origin2 = circle2->getWorldOrigin();
		float distanceSquared = origin1.Subtract(origin2).SqrMagnitude();
		return distanceSquared <= (circle1->radius + circle2->radius) * (circle1->radius + circle2->radius);
	}
	static void ResolveCollistionRectAndCircle(RectColliderComponent* rect, CircleColliderComponent* circle, PhysicComponent* physic2) {
		if (!CheckCollisionRectAndCircle(rect, circle)) return;
		Vector2D circleOrigin = circle->getWorldOrigin();
		Vector2D rectOrigin = rect->getWorldOrigin();
		float closestX = fmax(rectOrigin.x - rect->size.x / 2, fmin(circleOrigin.x, rectOrigin.x + rect->size.x / 2));
		float closestY = fmax(rectOrigin.y - rect->size.y / 2, fmin(circleOrigin.y, rectOrigin.y + rect->size.y / 2));
		Vector2D direction = Vector2D(circleOrigin.x - closestX, circleOrigin.y - closestY);
		float overlap = circle->radius - direction.Magnitude();
		circle->transform->position += direction.Normalize() * overlap;
	}
	static void ResolveCollistionRectAndCircleWithVelocity(RectColliderComponent* rect, CircleColliderComponent* circle, PhysicComponent* physic2) {
		if (!CheckCollisionRectAndCircle(rect, circle)) return;
		Vector2D circleOrigin = circle->getWorldOrigin();
		Vector2D rectOrigin = rect->getWorldOrigin();
		float closestX = fmax(rectOrigin.x - rect->size.x / 2, fmin(circleOrigin.x, rectOrigin.x + rect->size.x / 2));
		float closestY = fmax(rectOrigin.y - rect->size.y / 2, fmin(circleOrigin.y, rectOrigin.y + rect->size.y / 2));
		Vector2D direction = Vector2D(circleOrigin.x - closestX, circleOrigin.y - closestY);
		float overlap = circle->radius - direction.Magnitude();
		if (abs(direction.x) > abs(direction.y)) {
			physic2->velocity = Vector2D(-physic2->velocity.x, physic2->velocity.y);
		}
		else if (abs(direction.x) < abs(direction.y)) {
			physic2->velocity = Vector2D(physic2->velocity.x, -physic2->velocity.y);
		}
		else {
			physic2->velocity = Vector2D(-physic2->velocity.x, -physic2->velocity.y);
		}
		circle->transform->position += direction.Normalize() * overlap;
	}
	static void ResolveCollisionCircleAndCircle(CircleColliderComponent* circle1, PhysicComponent* physic1, CircleColliderComponent* circle2, PhysicComponent* physic2) {
		if (!CheckCollisionCircleAndCircle(circle1, circle2)) return;
		Vector2D distance = circle1->getWorldOrigin();
		distance -= circle2->getWorldOrigin();
		float overlap = circle1->radius + circle2->radius - distance.Magnitude();
		if (overlap == 0.0f) return;
		distance.Normalize();
		distance.Multiply(overlap / 2);
		circle1->transform->position += distance;
		circle2->transform->position -= distance;
	}
	static void ResolveCollisionWithVelocityCircleAndCircle(CircleColliderComponent* circle1, PhysicComponent* physic1, CircleColliderComponent* circle2, PhysicComponent* physic2) {
		if (!CheckCollisionCircleAndCircle(circle1, circle2)) return;
		Vector2D distance = circle1->getWorldOrigin();
		distance -= circle2->getWorldOrigin();
		float overlap = circle1->radius + circle2->radius - distance.Magnitude();
		distance.Normalize();

		float m1 = physic1->mass;
		float m2 = physic2->mass;

		Vector2D oldVelocity1 = physic1->velocity;
		Vector2D newVelocity1 = physic1->velocity;
		Vector2D oldVelocity2 = physic2->velocity;
		Vector2D newVelocity2 = physic2->velocity;
		Vector2D vDirection1 = distance;
		vDirection1.Multiply(oldVelocity1.x * distance.x + oldVelocity1.y * distance.y);
		newVelocity1 -= vDirection1;
		Vector2D vDirection2 = distance;
		vDirection2.Multiply(oldVelocity2.x * distance.x + oldVelocity2.y * distance.y);
		newVelocity2 -= vDirection2;

		newVelocity1 += Vector2D(oldVelocity1.x, oldVelocity1.y).Multiply((m1 - m2) / (m1 + m2)).Add(
			Vector2D(oldVelocity2.x, oldVelocity2.y).Multiply(2 * m2 / (m1 + m2)));
		newVelocity2 += Vector2D(oldVelocity2.x, oldVelocity2.y).Multiply((m2 - m1) / (m1 + m2)).Add(
			Vector2D(oldVelocity1.x, oldVelocity1.y).Multiply(2 * m1 / (m1 + m2)));

		/*Vector2D avgVDirection = vDirection1;
		avgVDirection += vDirection2;
		avgVDirection /= 2;*/

		/*newVelocity1 += avgVDirection;
		newVelocity2 += avgVDirection;*/

		physic1->velocity = newVelocity1;
		physic2->velocity = newVelocity2;

		distance.Multiply(overlap / 2);
		circle1->transform->position += distance;
		circle2->transform->position -= distance;
	}
};

