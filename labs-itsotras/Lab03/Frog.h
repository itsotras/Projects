#pragma once
#include "Actor.h"
#include <map>
#include <SDL2/SDL_scancode.h>


class Frog : public Actor {
public:
	Frog(Game* game);

	void OnProcessInput(const Uint8* keyState);

	void OnUpdate(float deltaTime);

	Vector2 startPos;

private:
	class SpriteComponent* mSprite;
	class CollisionComponent* mCollider;
	bool onLog = false;
	std::map<SDL_Scancode, bool> lastFrames;
};