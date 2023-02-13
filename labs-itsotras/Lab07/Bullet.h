#pragma once
#include "Actor.h"

class Bullet : public Actor {
public:
	Bullet(class Game* game, float speedMultiplier);
	void OnUpdate(float deltaTime);
private:
	float timer = 1.0f;
	class CollisionComponent* mCollider;
};