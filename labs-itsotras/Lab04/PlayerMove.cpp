#include "PlayerMove.h"
#include "Actor.h"
#include "Player.h"
#include "Goomba.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include <SDL2/SDL_scancode.h>
//#include "SDL2/SDL_mixer.h"
#include "Block.h"

PlayerMove::PlayerMove(Actor* owner) :MoveComponent(owner) {
}
PlayerMove::~PlayerMove() {
}
void PlayerMove::ProcessInput(const Uint8* keyState) {
	if (keyState[SDL_SCANCODE_LEFT]) {
		SetForwardSpeed(-1 *speed);
	}
	else if (keyState[SDL_SCANCODE_RIGHT]) {
		SetForwardSpeed(speed);
	}
	else {
		SetForwardSpeed(0.0f);
	}
	if (keyState[SDL_SCANCODE_SPACE]) {//this frame is True
		if (!mSpacePressed) {//last Frame is false
			if (!mInAir) {
				Mix_Chunk* myChunk = mOwner->GetGame()->GetSound("Assets/Sounds/Jump.wav");
				Mix_PlayChannel(-1, myChunk, 0);
				mYSpeed = jumpSpeed;
			}
			mInAir = true;
			mSpacePressed = true;//set lastFrame to true
		}
	}
	mSpacePressed = keyState[SDL_SCANCODE_SPACE];

}
void PlayerMove::Update(float deltaTime) {
	Vector2 pos = mOwner->GetPosition();
	pos.x += mForwardSpeed * deltaTime;//set position
	pos.y += mYSpeed * deltaTime;

	if (pos.y > limit) {
		mOwner->GetGame()->killPlayer();
	}
	if (pos.x >= edgeOfLevel) {
		Mix_HaltChannel(mOwner->GetGame()->getChannel());
		Mix_Chunk* myChunk = mOwner->GetGame()->GetSound("Assets/Sounds/StageClear.wav");
		Mix_PlayChannel(-1, myChunk, 0);
		mOwner->SetState(ActorState::Paused);
	}
	mOwner->SetPosition(pos);
	CollisionComponent* mCollider = mOwner->GetComponent<CollisionComponent>();
	bool collideWithBlocks = false;
	const std::vector<Block*> blocks = mOwner->GetGame()->GetBlocks();
	for (auto block : blocks) {
		CollisionComponent* blockCollider = block->GetCollider();
		Vector2 offset;
		CollSide status = mCollider->GetMinOverlap(blockCollider, offset);

		if (status != CollSide::None) {
			if (status == CollSide::Top && mYSpeed > 0.0f) {
				collideWithBlocks = true;
				mInAir = false;
				mYSpeed = 0.0f;
				mOwner->SetPosition(mOwner->GetPosition()+offset);
			}
			else {
				if (status == CollSide::Bottom) {
					if (mYSpeed < 0.0f) {
						mYSpeed = 0.0f;
					}
					Mix_Chunk* myChunk = mOwner->GetGame()->GetSound("Assets/Sounds/Bump.wav");
					Mix_PlayChannel(-1, myChunk, 0);
				}
				collideWithBlocks = true;
				mOwner->SetPosition(mOwner->GetPosition()+offset);
			}
		}
	}
	if (collideWithBlocks == false) {
		mInAir = true;
	}
	const std::vector<Goomba*> enemies = mOwner->GetGame()->GetEnemies();
	for (auto goomba : enemies) {
		if (goomba->getStomped()) {
			continue;
		}
		CollisionComponent* enemyCollider = goomba->GetCollider();
		Vector2 offset;
		CollSide status = mCollider->GetMinOverlap(enemyCollider, offset);
		if (status != CollSide::None) {
			if (status == CollSide::Top) {
				Mix_Chunk* myChunk = mOwner->GetGame()->GetSound("Assets/Sounds/Stomp.wav");
				Mix_PlayChannel(-1, myChunk, 0);
				goomba->Stomped();
				mYSpeed = -350.0f;
				mInAir = true;
			}
			else if ((status == CollSide::Right || status == CollSide::Left) && mInAir) {
				Mix_Chunk* myChunk = mOwner->GetGame()->GetSound("Assets/Sounds/Stomp.wav");
				Mix_PlayChannel(-1, myChunk, 0);
				goomba->Stomped();
				mYSpeed = -350.0f;
				mInAir = true;
			}
			else if ((status == CollSide::Right || status == CollSide::Left) && !mInAir) {
				mOwner->GetGame()->killPlayer();
			}
		}
	}
	mYSpeed += mGravity * deltaTime;
	float newXPos = mOwner->GetPosition().x - mOwner->GetGame()->getWindowWidth() / 2;
	if (newXPos >= mOwner->GetGame()->GetCameraPos().x) {
		mOwner->GetGame()->GetCameraPos().x = newXPos;
	}
	if (mOwner->GetGame()->GetCameraPos().x < 0.0f) {
		mOwner->GetGame()->GetCameraPos().x = 0.0f;
	}
	if (mOwner->GetPosition().x < mOwner->GetGame()->GetCameraPos().x) {
		Vector2 myPos = mOwner->GetGame()->GetCameraPos();
		myPos.y = mOwner->GetPosition().y;
		mOwner->SetPosition(myPos);
	}
	SetMyAnimation();
}

void PlayerMove::SetMyAnimation() {
	AnimatedSprite* myAnimator = mOwner->GetGame()->getPlayer()->getAnimator();
	if (mOwner->GetGame()->getPlayer()->getDeath()) {
		myAnimator->SetAnimation("dead");
	}
	else if (!mInAir) {//if i'm not in the air
		if (mForwardSpeed == 0.0f) {
			myAnimator->SetAnimation("idle");
		}
		else if (mForwardSpeed < 0.0f) {
			myAnimator->SetAnimation("runLeft");
		}
		else {
			myAnimator->SetAnimation("runRight");
		}
	}
	else {
		if (mForwardSpeed < 0.0f) {
			myAnimator->SetAnimation("jumpLeft");
		}
		else if (mForwardSpeed > 0.0f) {
			myAnimator->SetAnimation("jumpRight");
		}
		else {
			if (myAnimator->GetAnimName() == "idle" || myAnimator->GetAnimName() == "jumpRight" || myAnimator->GetAnimName() == "runRight") {
				myAnimator->SetAnimation("jumpRight");
			}
			else {
				myAnimator->SetAnimation("jumpLeft");
			}
		}
	}
}