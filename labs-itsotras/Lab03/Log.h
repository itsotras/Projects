#pragma once
#include "Actor.h"

class Log : public Actor {
public:
	Log(Game* game, char type, int line);
	~Log();
private:
	class SpriteComponent* mSprite;
	class WrappingMove* mWrapper;
	class CollisionComponent* mCollider;

};