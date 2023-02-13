#pragma once
#include "MoveComponent.h"
#include "Math.h"
#include "CollisionComponent.h"


enum MoveState {
	OnGround, 
	Jump, 
	Falling
};
class PlayerMove : public MoveComponent {
public:
	PlayerMove(Actor* owner);
	void Update(float deltaTime);
	void ProcessInput(const Uint8* keyState);
	void ChangeState(MoveState newState) { currentState = newState; }
	void UpdateOnGround(float deltaTime);
	void UpdateJump(float deltaTime);
	void UpdateFalling(float deltaTime);
	void PhysicsUpdate(float deltaTime);
	void AddForce(const Vector3& force) { mPendingForces += force; }
	void FixXYVelocity();

protected:
	CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);
private:
	MoveState currentState;
	//float mZSpeed = 0.0f;
	//const float GRAVITY = -980.0f;
	bool prevState = false;
	//const float JUMP_SPEED = 500.0f;
	Vector3 mVelocity;
	Vector3 mAcceleration;
	Vector3 mPendingForces;
	float mMass = 1.0f;
	Vector3 mGravity = Vector3(0.0f, 0.0f, -980.0f);
	Vector3 mJumpForce = Vector3(0.0f, 0.0f, 35000.0f);
	
};