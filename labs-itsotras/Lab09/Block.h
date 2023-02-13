#pragma once
#include "Actor.h"

class Block : public Actor {
public:
	Block(class Game* game);
	~Block();
private:
	float scale = 64.0f;
	class MeshComponent* mMesh;
	class CollisionComponent* mCollider;
};