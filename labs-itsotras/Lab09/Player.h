#pragma once
#include "Actor.h"

class Player : public Actor {
public:

	Player(class Game* game);
	class CollisionComponent* getCollider() { return mCollider; };
	class PlayerMove* mMover;
	void OnUpdate(float deltaTime);

private:
	class CollisionComponent* mCollider;
	float width = 50.0f;
	float height = 175.0f;
	float depth = 50.0f;
	
};