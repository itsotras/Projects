#pragma once
#include "Actor.h"

class Sword : public Actor {
public:
	Sword(class Game* owner);
	void changeSize(float width, float height, Vector2 offset);
	class CollisionComponent* getMyCollider();
private:

	float big_size = 28.0f;
	class CollisionComponent* mCollider;
};