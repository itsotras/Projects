#include "Effect.h"
#include "AnimatedSprite.h"
#include "Game.h"

Effect::Effect(class Game* game, Vector2 pos, std::string animName, std::string soundName):Actor(game) {
	SetPosition(pos);

	AnimatedSprite* mAnimator = new AnimatedSprite(this);
	mAnimator->LoadAnimations("Assets/Effects");
	mAnimator->SetAnimation(animName);
	lifeTime = mAnimator->GetAnimDuration(animName);
	Mix_Chunk* myChunk = GetGame()->GetSound(soundName);
	Mix_PlayChannel(-1, myChunk, 0);

}

void Effect::OnUpdate(float deltaTime) {
	lifeTime -= deltaTime;
	if (lifeTime <= 0.0f) {
		mState = ActorState::Destroy;
	}
}