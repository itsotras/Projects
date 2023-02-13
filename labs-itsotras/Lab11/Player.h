#pragma once
#include "Actor.h"

class Player : public Actor {
public:

	Player(class Game* game, Actor* parent);
	class CollisionComponent* getCollider() { return mCollider; };
	class PlayerMove* mMover;
	void OnUpdate(float deltaTime);
	void SetRespawnPos(Vector3 newPos) { RespawnPos = newPos; }
	Vector3 GetRespawnPos() { return RespawnPos; }

private:
	class CollisionComponent* mCollider;
	float width = 50.0f;
	float height = 175.0f;
	float depth = 50.0f;
	Vector3 RespawnPos = Vector3::Zero;
	
};