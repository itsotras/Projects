#pragma once
#include "Actor.h"

class Asteroid : public Actor {
public:
	Asteroid(Game* game);
	~Asteroid();

private:
	class SpriteComponent* mSprite;
	class MoveComponent* mMover;

	void OnUpdate(float deltaTime);
};