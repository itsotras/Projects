#pragma once
#include "Actor.h"
#include <vector>

class Enemy : public Actor {
public:
	Enemy(class Game* game);
private:
	float scale = 0.75f;
};