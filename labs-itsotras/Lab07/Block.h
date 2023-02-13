#pragma once
#include "Actor.h"

class Block : public Actor {
public:
	Block(Game* game, size_t index);
	~Block();
	void OnUpdate(float deltaTime);
	bool GetExploding() { return isExploding; }
	void Explode();
	class CollisionComponent* GetCollider() { return mCollider; }
private:
	bool isExploding = false;
	class CollisionComponent* mCollider;
};