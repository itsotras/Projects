#pragma once
#include "Actor.h"

class Collider : public Actor {
public:
	Collider(class Game* game, float width, float height);
	class CollisionComponent* GetCollisionComp();
private:
	class CollisionComponent* mCollision;

};