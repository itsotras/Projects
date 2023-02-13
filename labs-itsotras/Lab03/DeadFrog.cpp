#include "DeadFrog.h"
#include "SpriteComponent.h"
#include "Game.h"

DeadFrog::DeadFrog(Game* game) :Actor(game) {
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(game->GetTexture("Assets/Dead.png"));
}

void DeadFrog::OnUpdate(float deltaTime) {
	if (lifeTime > 0.0f) {
		lifeTime -= deltaTime;
	}
	else if (lifeTime <= 0.0f) {
		mState = ActorState::Destroy;
	}
}