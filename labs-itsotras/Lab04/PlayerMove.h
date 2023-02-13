#pragma once
#include "MoveComponent.h"

class PlayerMove : public MoveComponent {

public:
	PlayerMove(class Actor* owner);

	~PlayerMove();

	void ProcessInput(const Uint8* keyState);

	void Update(float deltaTime);

	void SetMyAnimation();

private:
	float mYSpeed = 0.0f;
	float limit = 480.0f;
	float mGravity = 2000.0f;
	float jumpSpeed = -700.0f;
	float edgeOfLevel = 6368.0f;
	float speed = 300.0f;
	bool mSpacePressed = false;
	bool mInAir = false;
};