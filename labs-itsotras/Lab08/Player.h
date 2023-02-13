#pragma once
#include "Actor.h"

class Player : public Actor {
public:

	Player(class Game* game);
	class CollisionComponent* getCollider() { return mCollider; };
	class PlayerMove* mMover;
	void OnUpdate(float deltaTime);
	class PlayerUI* getUI() { return myUI;  }

private:
	class CollisionComponent* mCollider;
	float width = 40.0f;
	float height = 25.0f;
	float depth = 15.0f;
	float scale = 0.75f;
	class PlayerUI* myUI;
	
};