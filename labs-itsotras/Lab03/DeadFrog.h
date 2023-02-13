#pragma once
#include "Actor.h"


class DeadFrog : public Actor {
public:
	DeadFrog(Game* game);

	void OnUpdate(float deltaTime);

private:
	float lifeTime = 0.5f;
	class SpriteComponent* mSprite;
};