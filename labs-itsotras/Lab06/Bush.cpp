#include "Bush.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "PathFinder.h"
#include "EnemyComponent.h"
#include "Effect.h"

Bush::Bush(Game* game) : Actor(game) {
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(mGame->GetTexture("Assets/Bush.png"));
	mCollider = new CollisionComponent(this);
	mCollider->SetSize(bush_size, bush_size);
	myEnemyComp = new EnemyComponent(this);
	myEnemyComp->setHP(1);
	myEnemyComp->SetOnDeath([this] {
		SetState(ActorState::Destroy);
		float x = GetPosition().x;
		float y = GetPosition().y;
		mGame->getPathFinder()->SetIsBlocked((size_t)(y/bush_size), (size_t)(x/bush_size), false);
		Effect* mEffect = new Effect(mGame, GetPosition(), "BushDeath", "Assets/Sounds/BushDie.wav");
		mEffect->GetPosition();
		});
}
