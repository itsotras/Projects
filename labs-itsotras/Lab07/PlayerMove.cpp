#include "PlayerMove.h"
#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include <SDL2/SDL_scancode.h>
#include "Block.h"
#include "SideBlock.h"
#include "Random.h"
#include "Bullet.h"
#include "SDL2/SDL_mixer.h"

PlayerMove::PlayerMove(Actor* owner) : MoveComponent(owner) {
	

	SetUpBlocks(0.0f, 4000.0f);
}

void PlayerMove::Update(float deltaTime) {
	if (speedTimer >= 0.0f) {
		speedTimer -= deltaTime;
	}
	else {
		speedMultiplier += .15f;
		speedTimer = 10.0f;
	}

	if (isRolling) {
		if (barrelRollTimer >= 0.0f) {
			barrelRollTimer -= deltaTime;
			mOwner->SetRollAngle(mOwner->GetRollAngle() + 8.0f * Math::Pi * deltaTime);
		}
		else {
			isRolling = false;
			mOwner->SetRollAngle(0.0f);
		}
	}

	//change the position of player
	Vector3 myPos = mOwner->GetPosition();
	myPos.y += yVelocity * deltaTime;
	myPos.z += zVelocity * deltaTime;
	myPos.y = Math::Clamp(myPos.y, -180.0f, 180.0f);
	myPos.z = Math::Clamp(myPos.z, -225.0f, 225.0f);
	mOwner->SetPosition(myPos+mVelocity * speedMultiplier * deltaTime);

	//calculate eyePos and targetPos
	Vector3 eyePos = mOwner->GetPosition() - (mOwner->GetForward(0.0f) * hDist);
	eyePos.z = 0.0f;
	Vector3 targetPos = mOwner->GetPosition() + (mOwner->GetForward(0.0f) * targetDist);
	Matrix4 camera = Matrix4::CreateLookAt(eyePos, targetPos, Vector3::UnitZ);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(camera);

	//check if I have collided with any blocks
	TestBlockCollision();

	if (mOwner->GetPosition().x >= 500.0f*sideBlockCount) {
		SetUpBlocks(500.0f * sideBlockCount + 4000.0f, 500.0f * sideBlockCount + 4001.0f);
		sideBlockCount++;
	}
}

void PlayerMove::ProcessInput(const Uint8* keyState) {
	yVelocity = 0.0f;
	zVelocity = 0.0f;
	if (keyState[SDL_SCANCODE_W]&&wDown) {
		zVelocity += speed * speedMultiplier;
	}
	if (keyState[SDL_SCANCODE_S]&&sDown) {
		zVelocity -= speed * speedMultiplier;
	}
	if (keyState[SDL_SCANCODE_A]&&aDown) {
		yVelocity -= speed * speedMultiplier;
	}
	if (keyState[SDL_SCANCODE_D]&&dDown) {
		yVelocity += speed * speedMultiplier;
	}
	if (keyState[SDL_SCANCODE_Q]&& !qDown &&!isRolling) {
		DoABarrelRoll();
	}
	if (keyState[SDL_SCANCODE_SPACE] && !spaceDown) {
		Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Shoot.wav"), 0);
		Bullet* myBullet = new Bullet(mOwner->GetGame(), speedMultiplier);
		myBullet->SetPosition(mOwner->GetPosition());
	}
	aDown = keyState[SDL_SCANCODE_A];
	wDown = keyState[SDL_SCANCODE_W];
	sDown = keyState[SDL_SCANCODE_S];
	dDown = keyState[SDL_SCANCODE_D];
	spaceDown = keyState[SDL_SCANCODE_SPACE];
	
}

void PlayerMove::DoABarrelRoll() {
	if (!isRolling) {
		isRolling = true;
		Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Boost.wav"), 0);
		if (mOwner->GetGame()->GetPlayer()->GetShield() < 3) {
			if (mOwner->GetGame()->GetPlayer()->GetShield() == 1) {
				Mix_HaltChannel(damageChannel);
			}
			mOwner->GetGame()->GetPlayer()->SetShield(mOwner->GetGame()->GetPlayer()->GetShield() + 1);
		}
		barrelRollTimer = .5f;
	}

}

void PlayerMove::SetUpBlocks(float low, float high) {
	for (float i = low; i < high; i += 500.0f) {

		int topTexture = 7;
		if ((int)i % 1000 == 0) {
			topTexture = 6;
			if (i > 0.0f) {
				std::string file = "Assets/Blocks/" + std::to_string((int)(i / 1000.0f));

				if (i > 20000.0f) {
					file = "Assets/Blocks/" + std::to_string(Random::GetIntRange(1, 20));
				}

				file += ".txt";
				mOwner->GetGame()->LoadBlocks(file, i);
			}
		}

		SideBlock* top = new SideBlock(mOwner->GetGame(), topTexture);
		top->SetPosition(Vector3(i, 0.0f, 500.0f));

		int sideTexture = 0;
		if ((int)i % 2000 == 500) {
			sideTexture = 1;
		}
		else if ((int)i % 2000 == 1000) {
			sideTexture = 2;
		}

		SideBlock* left = new SideBlock(mOwner->GetGame(), sideTexture);
		left->SetPosition(Vector3(i, -500.0f, 0.0f));

		SideBlock* right = new SideBlock(mOwner->GetGame(), sideTexture);
		right->SetPosition(Vector3(i, 500.0f, 0.0f));
		right->SetRotation(Math::Pi);

		SideBlock* bottom = new SideBlock(mOwner->GetGame(), 5);
		bottom->SetPosition(Vector3(i, 0.0f, -500.0f));

	}
}

void PlayerMove::TestBlockCollision() {
	CollisionComponent* mCollider = mOwner->GetComponent<CollisionComponent>();
	int blocksTouched = 0;
	for (auto block : mOwner->GetGame()->GetBlocks()) {
		CollisionComponent* blockCollider = block->GetCollider();
		if (mCollider->Intersect(blockCollider)) {
			block->Explode();
			blocksTouched += 1;
		}
	}
	if (blocksTouched > 0) {
		TakeDamage();
	}
}

void PlayerMove::TakeDamage() {
	if (mOwner->GetGame()->GetPlayer()->GetShield() > 1) {
		Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/ShipHit.wav"), 0);
	}
	mOwner->GetGame()->GetPlayer()->SetShield(mOwner->GetGame()->GetPlayer()->GetShield() - 1);

	if (mOwner->GetGame()->GetPlayer()->GetShield() == 1) {
		damageChannel = Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/DamageAlert.ogg"), -1);
	}
	else if (mOwner->GetGame()->GetPlayer()->GetShield() <= 0) {
		Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/ShipDie.wav"), 0);
		Mix_HaltChannel(mOwner->GetGame()->GetLoopChannel());
		Mix_HaltChannel(damageChannel);
		mOwner->SetState(ActorState::Paused);
	}
}