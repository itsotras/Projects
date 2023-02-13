#pragma once
#include "Actor.h"

class Block: public Actor{
public:
	Block(Game* game, char type);
	~Block();

	class CollisionComponent* GetCollider();
	

private:
	class SpriteComponent* mSprite;
	float blockSize = 32.0f;
	class CollisionComponent* mCollider;

};