#pragma once
#include "Actor.h"

class LaserMine : public Actor {
public:
	LaserMine(class Game* game);
private:
	class LaserComponent* mLaser;
};