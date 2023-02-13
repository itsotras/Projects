#pragma once
#include "Actor.h"

class Bush : public Actor {
public:
	Bush(class Game* game);
private:
	class SpriteComponent* mSprite;
	class CollisionComponent* mCollider;
	float bush_size = 32.0f;
	class EnemyComponent* myEnemyComp;
};