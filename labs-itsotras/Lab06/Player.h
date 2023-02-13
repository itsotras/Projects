#pragma once
#include "Actor.h"

class Player : public Actor {
public:
	Player(class Game* game);
	~Player();
	void OnUpdate(float deltaTime);
	class AnimatedSprite* getAnimator();
private:
	class AnimatedSprite* mAnimated;
	class CollisionComponent* mCollider;
	class PlayerMove* mMover;
	float playerSize = 20.0f;
	bool isDead = false;
};