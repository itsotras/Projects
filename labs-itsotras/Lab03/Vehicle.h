#pragma once
#include "Actor.h"

class Vehicle : public Actor {
public:
	Vehicle(Game* game, char type, int line);
	~Vehicle();
	void OnUpdate(float deltaTime);

private:
	float mForwardSpeed = 100.0f;
	class SpriteComponent* mSprite;
	class CollisionComponent* mCollider;
	class WrappingMove* mWrapper;
};