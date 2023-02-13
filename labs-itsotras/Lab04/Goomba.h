#pragma once
#include "Actor.h"

class Goomba : public Actor {
public:
	Goomba(Game* game);
	~Goomba();
	class CollisionComponent* GetCollider();
	void OnUpdate(float deltaTime);
	void Stomped();
	bool getStomped();
private:
	class AnimatedSprite* mAnimated;
	class CollisionComponent* mCollider;
	class GoombaMove* mMover;
	bool stomped = false;
	float goombaSize = 32.0f;
	float lifeTime = .25f;
};