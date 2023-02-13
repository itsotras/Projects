#pragma once
#include "Actor.h"

class Player : public Actor {
public:
	Player(class Game* game);
	~Player();
	void OnUpdate(float deltaTime);
	void setDeath();
	bool getDeath();
	class AnimatedSprite* getAnimator();
private:
	class AnimatedSprite* mAnimated;
	class CollisionComponent* mCollider;
	class PlayerMove* mMover;
	float playerSize = 32.0f;
	bool isDead = false;
};