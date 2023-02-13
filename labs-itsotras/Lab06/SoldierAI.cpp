#include "SoldierAI.h"
#include "Actor.h"
#include "Soldier.h"
#include "Game.h"
#include "PathFinder.h"
#include "PathNode.h"
#include "AnimatedSprite.h"
#include <algorithm>

SoldierAI::SoldierAI(Actor* owner)
:SpriteComponent(owner)
{
	SetIsVisible(false); // Uncomment to hide debug paths
}

void SoldierAI::Setup(PathNode* start, PathNode* end)
{
	mPatrolStart = start;
	mPatrolEnd = end;
	mOwner->GetGame()->getPathFinder()->CalculatePath(mPatrolStart, mPatrolEnd, mPath);
	mPrev = mPatrolStart;
	mNext = mPath.back();
	mPath.pop_back();
	myDirection = Vector2::Normalize(mNext->GetPosition() - mPrev->GetPosition());
}

void SoldierAI::Update(float deltaTime)
{
	if (isPaused) {
		pauseTimer += deltaTime;
		if (pauseTimer >= STUN_DURATION) {
			isPaused = false;
			mOwner->GetComponent<AnimatedSprite>()->SetIsPaused(false);
		}
	}
	else {
		Vector2 mPos = mOwner->GetPosition();
		mPos += SOLDIER_SPEED * myDirection * deltaTime;
		mOwner->SetPosition(mPos);
		if (Vector2::Distance(mOwner->GetPosition(), mNext->GetPosition()) <= 1.0f) {
			mOwner->SetPosition(mNext->GetPosition());
			if (!mPath.empty()) {
				mPrev = mNext;
				mNext = mPath.back();
				mPath.pop_back();
				myDirection = Vector2::Normalize(mNext->GetPosition() - mOwner->GetPosition());
			}
			else {
				Setup(mPatrolEnd, mPatrolStart);
			}
		}
		UpdateAnimator();
	}
	

}

// This helper is to just debug draw the soldier's path to visualize it
// (only called if this component is set to visible)
void SoldierAI::Draw(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	Vector2 cameraPos = mOwner->GetGame()->GetCameraPos();
	
	// Draw from current position to next
	SDL_RenderDrawLine(renderer,
					   static_cast<int>(mOwner->GetPosition().x - cameraPos.x),
					   static_cast<int>(mOwner->GetPosition().y - cameraPos.y),
					   static_cast<int>(mNext->GetPosition().x - cameraPos.x),
					   static_cast<int>(mNext->GetPosition().y - cameraPos.y));

	// Draw from next to first node on path
	if (!mPath.empty())
	{
		// Draw from current position to next
		SDL_RenderDrawLine(renderer,
						   static_cast<int>(mNext->GetPosition().x - cameraPos.x),
						   static_cast<int>(mNext->GetPosition().y - cameraPos.y),
						   static_cast<int>(mPath.back()->GetPosition().x - cameraPos.x),
						   static_cast<int>(mPath.back()->GetPosition().y - cameraPos.y));
	}
	
	// Draw each node on the path
	if (mPath.size() > 1)
	{
		for (size_t i = 0; i < mPath.size() - 1; i++)
		{
			SDL_RenderDrawLine(renderer,
							   static_cast<int>(mPath[i]->GetPosition().x - cameraPos.x),
							   static_cast<int>(mPath[i]->GetPosition().y - cameraPos.y),
							   static_cast<int>(mPath[i + 1]->GetPosition().x - cameraPos.x),
							   static_cast<int>(mPath[i + 1]->GetPosition().y - cameraPos.y));
		}
	}
}

void SoldierAI::UpdateAnimator() {
	AnimatedSprite* mAnimator = mOwner->GetComponent<AnimatedSprite>();
	if (myDirection.x == 1.0f ){
		mAnimator->SetAnimation("WalkRight");
	}
	else if (myDirection.x == -1.0f) {
		mAnimator->SetAnimation("WalkLeft");
	}
	else if (myDirection.y == -1.0f) {
		mAnimator->SetAnimation("WalkUp");
	}
	else {
		mAnimator->SetAnimation("WalkDown");
	}
}
