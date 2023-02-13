#pragma once
#include "Actor.h"

class Laser : public Actor {
public:
	Laser(Game* game);

private:
	class SpriteComponent* mSprite;
	class MoveComponent* mMover;

	void OnUpdate(float deltaTime);

	float lifeTime = 1.0f;
};