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
	else if(currentState == MoveState::Jump) {
		UpdateJump(deltaTime);
	}
	else if(currentState == MoveState::WallClimb) {
		UpdateWallClimb(deltaTime);
	}
	else {
		UpdateWallRun(deltaTime);
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
		AddForce(mOwner->GetForward() * 700.0f);
	}
	if (keyState[SDL_SCANCODE_S]) {
		AddForce(mOwner->GetForward() * -700.0f);
	}

	if (keyState[SDL_SCANCODE_D]) {
		AddForce(mOwner->GetRight() * 700.0f);
	}
	if (keyState[SDL_SCANCODE_A]) {
		AddForce(mOwner->GetRight() * -700.0f);
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
		else {
			if (status != CollSide::Bottom) {
				if(CanWallClimb(status)){
					mWallClimbTimer = 0.0f;
					currentState = MoveState::WallClimb;
					return;
				}
			}
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
		else {
			if (status != CollSide::Top) {
				if (CanWallClimb(status)) {
					mWallClimbTimer = 0.0f;
					currentState = MoveState::WallClimb;
					return;
				}
				else if (CanWallRun(status)) {
					mWallRunTimer = 0.0f;
					currentState = MoveState::WallRun;
					mOwner->GetComponent<CameraComponent>()->SetOnWall(true);
					mOwner->GetComponent<CameraComponent>()->SetSide(status);
					return;
				}
			}
		}
	}
	if (mVelocity.z <= 0.0f) {
		ChangeState(MoveState::OnGround);
	}

}

CollSide PlayerMove::FixCollision(CollisionComponent* self, CollisionComponent* block) {
	Vector3 offset;
	float x = 0.0f;
	float y = 0.0f;
	CollSide status = self->GetMinOverlap(block, offset);
	if (status != CollSide::None) {
		if (status == CollSide::Left) {
			y = -1.0f;
		}
		else if (status == CollSide::Left) {
			y = 1.0f;
		}
		else if (status == CollSide::Front) {
			x = 1.0f;
		}
		else if (status == CollSide::Back) {
			x = -1.0f;
		}
		Vector3 normalForce = normalMagnitude * Vector3(x, y, 0.0f);

		mOwner->SetPosition(mOwner->GetPosition()+offset);
		AddForce(normalForce);
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
	if (currentState == MoveState::OnGround || currentState == MoveState::WallClimb) {
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

void PlayerMove::UpdateWallClimb(float deltaTime) {
	mWallClimbTimer += deltaTime;
	AddForce(mGravity);
	if (mWallClimbTimer < 0.4f) {
		AddForce(mWallClimb);
	}
	PhysicsUpdate(deltaTime);
	bool collided = false;
	CollisionComponent* mCollider = mOwner->GetComponent<CollisionComponent>();
	for (auto& block : mOwner->GetGame()->getBlocks()) {
		CollSide status = FixCollision(mCollider, block->GetComponent<CollisionComponent>());
		if (status != CollSide::None) {
			if (status != CollSide::Top && status != CollSide::Bottom) {
				collided = true;
			}
		}
	}

	if (!collided || mVelocity.z <= 0.0f) {
		mVelocity.z = 0.0f;
		currentState = MoveState::Falling;
	}

}

bool PlayerMove::CanWallClimb(CollSide status) {
	if (status == CollSide::None) {
		return false;
	}
	Vector3 newVector = GetNormalOfBlock(status);
	Vector3 normalized = mOwner->GetForward();
	float dot = Vector3::Dot(normalized, newVector);
	if (dot < -1.15f || dot > -.85f) {
		return false;
	}
	Vector2 temp = Vector2(mVelocity.x, mVelocity.y);
	Vector2 xyVelocity = Vector2::Normalize(temp);
	Vector2 normal = Vector2(newVector.x, newVector.y);
	float dot1 = Vector2::Dot(xyVelocity, normal);
	if (dot1 < -1.15f || dot1 > -.85f) {
		return false;
	}

	if (temp.Length() < 350.0f) {
		return false;
	}

	return true;
}

void PlayerMove::UpdateWallRun(float deltaTime) {
	mWallRunTimer += deltaTime;
	AddForce(mGravity);
	if (mWallRunTimer < 0.4f) {
		AddForce(mWallRunForce);
	}
	PhysicsUpdate(deltaTime);
	CollisionComponent* mCollider = mOwner->GetComponent<CollisionComponent>();
	for (auto& block : mOwner->GetGame()->getBlocks()) {
		FixCollision(mCollider, block->GetComponent<CollisionComponent>());
	}

	if (mVelocity.z <= 0.0f) {
		mVelocity.z = 0.0f;
		mOwner->GetComponent<CameraComponent>()->SetOnWall(false);
		currentState = MoveState::Falling;
	}
}
bool PlayerMove::CanWallRun(CollSide status) {
	if (status == CollSide::None) {
		return false;
	}
	Vector3 newVector = GetNormalOfBlock(status);
	Vector3 normalized = mOwner->GetForward();
	float dot = Vector3::Dot(normalized, newVector);
	if (dot < -.3f || dot > .3f) {
		return false;
	}
	Vector2 temp = Vector2(mVelocity.x, mVelocity.y);
	Vector2 xyVelocity = Vector2::Normalize(temp);
	Vector2 forward = Vector2(normalized.x, normalized.y);
	float dot1 = Vector2::Dot(xyVelocity, forward);
	if (dot1 > 1.15f || dot1 < .85f) {
		return false;
	}

	if (temp.Length() < 350.0f) {
		return false;
	}

	return true;
}

Vector3 PlayerMove::GetNormalOfBlock(CollSide status) {
	float x = 0.0f;
	float y = 0.0f;
	if (status == CollSide::Front) {
		x = 1.0f;
	}
	else if (status == CollSide::Back) {
		x = -1.0f;
	}
	else if (status == CollSide::Left) {
		y = -1.0f;
	}
	else {
		y = 1.0f;
	}
	return Vector3(x, y, 0.0f);
}