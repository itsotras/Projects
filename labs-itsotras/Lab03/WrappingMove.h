#pragma once
#include "MoveComponent.h"
#include "Math.h"

class WrappingMove : public MoveComponent {
public:
	WrappingMove(Actor* owner);

	void Update(float deltaTime);

	void setDirection(Vector2 newDirection);

	Vector2 getForwardVector();

private:
	Vector2 facingDirection;
	
};