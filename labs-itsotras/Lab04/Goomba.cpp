#include "Goomba.h"
#include "GoombaMove.h"
#include "AnimatedSprite.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "Game.h"


Goomba::Goomba(Game* game):Actor(game) {

	mAnimated = new AnimatedSprite(this);
	std::vector<SDL_Texture*> walkAnim{
	GetGame()->GetTexture("Assets/Goomba/Walk0.png"),
	GetGame()->GetTexture("Assets/Goomba/Walk1.png")
	};
	std::vector<SDL_Texture*> deadAnim{
	GetGame()->GetTexture("Assets/Goomba/Dead.png")
	};
	mAnimated->AddAnimation("walk", walkAnim);
	mAnimated->AddAnimation("dead", deadAnim);
	mAnimated->SetAnimation("walk");

	mCollider = new CollisionComponent(this);
	mCollider->SetSize(goombaSize, goombaSize);

	mMover = new GoombaMove(this);

	mGame->AddEnemy(this);
}

Goomba::~Goomba() {
	mGame->RemoveEnemy(this);
}

CollisionComponent* Goomba::GetCollider() {
	return mCollider;
}
void Goomba::OnUpdate(float deltaTime) {

	if (stomped) {
		mAnimated->SetAnimation("dead");

		mMover->SetForwardSpeed(0.0f);
		if (lifeTime > 0.0f) {
			lifeTime -= deltaTime;
		}
		else if (lifeTime <= 0.0f) {
			mState = ActorState::Destroy;
		}
	}

}

void Goomba::Stomped() {
	stomped = true;
}
bool Goomba::getStomped() {
	return stomped;
}