#pragma once
#include "MoveComponent.h"
#include "Math.h"
#include "CollisionComponent.h"


enum MoveState {
	OnGround, 
	Jump, 
	Falling, 
	WallClimb,
	WallRun
};
class PlayerMove : public MoveComponent {
public:
	PlayerMove(Actor* owner);
	void Update(float deltaTime);
	void ProcessInput(const Uint8* keyState);
	
protected:
	CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);
private:
	void UpdateOnGround(float deltaTime);
	void UpdateJump(float deltaTime);
	void UpdateFalling(float deltaTime);
	void UpdateWallClimb(float deltaTime);
	void UpdateWallRun(float deltaTime);
	bool CanWallClimb(CollSide status);
	bool CanWallRun(CollSide status);
	Vector3 GetNormalOfBlock(CollSide status);
	void PhysicsUpdate(float deltaTime);
	void AddForce(const Vector3& force) { mPendingForces += force; }
	void FixXYVelocity();
	void ChangeState(MoveState newState) { currentState = newState; }

	MoveState currentState;
	bool prevState = false;
	Vector3 mVelocity;
	Vector3 mAcceleration;
	Vector3 mPendingForces;
	float mMass = 1.0f;
	Vector3 mGravity = Vector3(0.0f, 0.0f, -980.0f);
	Vector3 mWallClimb = Vector3(0.0f, 0.0f, 1800.0f);
	Vector3 mJumpForce = Vector3(0.0f, 0.0f, 35000.0f);
	Vector3 mWallRunForce = Vector3(0.0f, 0.0f, 1200.0f);
	float mWallClimbTimer = 0.0f;
	float normalMagnitude = 700.0f;
	float mWallRunTimer = 0.0f;
};