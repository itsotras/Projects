#include "PlayerMove.h"
#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include <SDL2/SDL_scancode.h>
#include "SDL2/SDL_mixer.h"
#include "CameraComponent.h"

PlayerMove::PlayerMove(Actor* owner) : MoveComponent(owner) {
	ChangeState(MoveState::Falling);
}

void PlayerMove::Update(float deltaTime) {

	//change the position of player
	
	if (currentState == MoveState::Falling) {
		UpdateFalling(deltaTime);
	}
	else if (currentState == MoveState::OnGround) {
		UpdateOnGround(deltaTime);
	}
	else {
		UpdateJump(deltaTime);
	}
}

void PlayerMove::ProcessInput(const Uint8* keyState) {
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	float angularSpeed = x / 500.0f * Math::Pi * 10.0f;
	SetAngularSpeed(angularSpeed);
	float pitchSpeed = y / 500.0f * Math::Pi * 10.0f;
	CameraComponent* camera = mOwner->GetComponent<CameraComponent>();
	camera->SetPitchSpeed(pitchSpeed);


	if (keyState[SDL_SCANCODE_W]) {
		AddForce(mOwner->GetForward(mOwner->GetRotation()) * 700.0f);
	}
	if (keyState[SDL_SCANCODE_S]) {
		AddForce(mOwner->GetForward(mOwner->GetRotation()) * -700.0f);
	}

	if (keyState[SDL_SCANCODE_D]) {
		AddForce(mOwner->GetRight(mOwner->GetRotation()) * 700.0f);
	}
	if (keyState[SDL_SCANCODE_A]) {
		AddForce(mOwner->GetRight(mOwner->GetRotation()) * -700.0f);
	}

	if (keyState[SDL_SCANCODE_SPACE] && !prevState && currentState == OnGround) {
		AddForce(mJumpForce);
		ChangeState(MoveState::Jump);
	}
	prevState = keyState[SDL_SCANCODE_SPACE];
}

void PlayerMove::UpdateOnGround(float deltaTime) {
	PhysicsUpdate(deltaTime);

	CollisionComponent* mCollider = mOwner->GetComponent<CollisionComponent>();
	bool onGround = false;
	for (auto& block : mOwner->GetGame()->getBlocks()) {
		CollSide status = FixCollision(mCollider, block->GetComponent<CollisionComponent>());
		if (status == CollSide::Top) {
			onGround = true;
		}
	}
	if (!onGround) {
		ChangeState(MoveState::Falling);
	}
}
void PlayerMove::UpdateFalling(float deltaTime) {
	AddForce(mGravity);
	PhysicsUpdate(deltaTime);
	CollisionComponent* mCollider = mOwner->GetComponent<CollisionComponent>();
	for (auto &block : mOwner->GetGame()->getBlocks()) {
		CollSide status = FixCollision(mCollider, block->GetComponent<CollisionComponent>());
		if (status == CollSide::Top) {
			mVelocity.z = 0.0f;
			ChangeState(MoveState::OnGround);
		}
	}
}
void PlayerMove::UpdateJump(float deltaTime) {
	AddForce(mGravity);
	PhysicsUpdate(deltaTime);
	CollisionComponent* mCollider = mOwner->GetComponent<CollisionComponent>();
	for (auto& block : mOwner->GetGame()->getBlocks()) {
		CollSide status = FixCollision(mCollider, block->GetComponent<CollisionComponent>());
		if (status == CollSide::Bottom) {
			mVelocity.z = 0.0f;
		}
	}
	if (mVelocity.z <= 0.0f) {
		ChangeState(MoveState::OnGround);
	}

}

CollSide PlayerMove::FixCollision(CollisionComponent* self, CollisionComponent* block) {
	Vector3 offset;
	CollSide status = self->GetMinOverlap(block, offset);
	if (status != CollSide::None) {
		mOwner->SetPosition(mOwner->GetPosition()+offset);
	}
	return status;
}

void PlayerMove::PhysicsUpdate(float deltaTime) {
	mAcceleration = mPendingForces * (1.0f / mMass);
	mVelocity += mAcceleration * deltaTime;
	FixXYVelocity();
	mOwner->SetPosition(mOwner->GetPosition() + mVelocity * deltaTime);
	mOwner->SetRotation(mOwner->GetRotation() + GetAngularSpeed() *deltaTime);
	mPendingForces = Vector3::Zero;
}

void PlayerMove::FixXYVelocity() {
	Vector2 xyVelocity = Vector2(mVelocity.x, mVelocity.y);

	if (xyVelocity.Length() > 400.0f) {
		xyVelocity.Normalize();
		xyVelocity *= 400.0f;
	}
	if (currentState == MoveState::OnGround) {
		float brakingFactor = 0.9f;
		if (Math::Abs(mAcceleration.x) <= 1.0f ) {
			xyVelocity.x *= brakingFactor;
		}
		if (Math::Abs(mAcceleration.y) <= 1.0f) {
			xyVelocity.y *= brakingFactor;
		}

		if (mAcceleration.x < 0.0f && xyVelocity.x > 0.0f ) {
			xyVelocity.x *= brakingFactor;
		}
		else if (mAcceleration.x > 0.0f && xyVelocity.x < 0.0f) {
			xyVelocity.x *= brakingFactor;
		}

		if (mAcceleration.y < 0.0f && xyVelocity.y > 0.0f) {
			xyVelocity.y *= brakingFactor;
		}
		else if (mAcceleration.y > 0.0f && xyVelocity.y < 0.0f) {
			xyVelocity.y *= brakingFactor;
		}

	}

	mVelocity.x = xyVelocity.x;
	mVelocity.y = xyVelocity.y;
}
