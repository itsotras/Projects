#include "Soldier.h"	
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "PathNode.h"
#include "SoldierAI.h"
#include "EnemyComponent.h"
#include "Effect.h"

Soldier::Soldier(Game* game, PathNode* start, PathNode* end) :Actor(game) {
	mCollider = new CollisionComponent(this);
	mCollider->SetSize(soldier_size, soldier_size);

	myEnemyComp = new EnemyComponent(this);
	myEnemyComp->setHP(2);	

	mAnimator = new AnimatedSprite(this);
	mAnimator->LoadAnimations("Assets/Soldier");
	mAnimator->SetAnimation("WalkDown");
	mAnimator->SetAnimFPS(5.0f);

	myAI = new SoldierAI(this);
	myAI->Setup(start, end);

	myEnemyComp->SetOnDamage([this] {
		mAnimator->SetIsPaused(true);
		myAI->setPaused(true);
		Effect* mEffect = new Effect(mGame, GetPosition(), "Hit", "Assets/Sounds/EnemyHit.wav");
		mEffect->GetPosition();
		});
	myEnemyComp->SetOnDeath([this] {
		Effect* mEffect = new Effect(mGame, GetPosition(), "Death", "Assets/Sounds/EnemyDie.wav");
		mEffect->GetPosition();
		mState = ActorState::Destroy;
		});
}
