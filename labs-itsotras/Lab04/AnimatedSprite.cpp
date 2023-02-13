#include "AnimatedSprite.h"
#include "Actor.h"
#include "Game.h"

AnimatedSprite::AnimatedSprite(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
{
}

void AnimatedSprite::Update(float deltaTime)
{
	if (mAnimName.empty()) {
		//do nothing
	}
	else {
		if (!mIsPaused) {
			mAnimTimer += mAnimFPS * deltaTime;
		}
		while (mAnimTimer >= (float)(mAnims[mAnimName].size())) {
			mAnimTimer -= (float)(mAnims[mAnimName].size());
		}
		this->SetTexture(mAnims[mAnimName][(unsigned long)(mAnimTimer)]);
	}
}

void AnimatedSprite::AddAnimation(const std::string& name,
								  const std::vector<SDL_Texture*>& images)
{
	mAnims.emplace(name, images);
}
