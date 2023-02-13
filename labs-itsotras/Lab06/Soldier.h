#pragma once
#include "Actor.h"

class Soldier : public Actor {
public:
	Soldier(class Game* game, class PathNode* start, class PathNode* end);
private:
	class AnimatedSprite* mAnimator;
	class CollisionComponent* mCollider;
	float soldier_size = 32.0f;
	class PathNode* start;
	class PathNode* end;
	class SoldierAI* myAI;
	class EnemyComponent* myEnemyComp;
};