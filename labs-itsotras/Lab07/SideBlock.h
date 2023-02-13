#pragma once
#include "Actor.h"

class SideBlock : public Actor {
public:
	SideBlock(class Game* game, size_t index);
	void OnUpdate(float deltaTime);
private:
	float scale = 500.0f;
};