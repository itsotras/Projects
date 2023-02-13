#pragma once
#include "MoveComponent.h"
#include "Math.h"

class PlayerMove : public MoveComponent {
public:
	PlayerMove(Actor* owner);
	void Update(float deltaTime);
	void ProcessInput(const Uint8* keyState);
	void DoABarrelRoll();
	void SetUpBlocks(float low, float high);
	void TestBlockCollision();
	void TakeDamage();

private:
	Vector3 mVelocity = Vector3(400.0f, 0.0f, 0.0f);
	float zVelocity = 0.0f;
	float yVelocity = 0.0f;

	float speed = 300.0f;
	float speedMultiplier = 1.0f;
	float speedTimer = 10.0f;

	float hDist = 300.0f;
	float targetDist = 20.0f;

	bool aDown = false;
	bool wDown = false;
	bool sDown = false;
	bool dDown = false;
	bool spaceDown = false;
	bool qDown = false;

	int sideBlockCount = 0;

	int damageChannel = -1;

	float barrelRollTimer = .5f;
	bool isRolling = false;
};