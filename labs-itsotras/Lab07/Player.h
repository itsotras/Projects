#pragma once
#include "Actor.h"

class Player : public Actor {
public:

	Player(class Game* game);
	class CollisionComponent* getCollider() { return mCollider; };
	class PlayerMove* mMover;
	void OnUpdate(float deltaTime);
	int GetShield() { return shield; }
	void SetShield(int newShield) { shield = newShield; }

private:
	class CollisionComponent* mCollider;
	float width = 40.0f;
	float height = 25.0f;
	float depth = 15.0f;
	float scale = 2.0f;
	int shield = 3;
	float cooldown = 0.0f;
	class HUD* myHud;
};