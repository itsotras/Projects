#include "WrappingMove.h"
#include "Game.h"
#include "Actor.h"

WrappingMove::WrappingMove(Actor* owner) :MoveComponent(owner) {
	facingDirection.x = 0.0f;
	facingDirection.y = 0.0f;
}

void WrappingMove::Update(float deltaTime) {

	Vector2 pos = mOwner->GetPosition();

	Vector2 velocity = facingDirection * mForwardSpeed;//calculate the velocity

	pos += velocity * deltaTime;//set position

	if (pos.x < 0.0f) {
		pos.x = mOwner->GetGame()->getWindowWidth();
	}
	else if (pos.x > mOwner->GetGame()->getWindowWidth()) {
		pos.x = 0.0f;
	}

	mOwner->SetPosition(pos);
}

void WrappingMove::setDirection(Vector2 newDirection) {

	facingDirection = newDirection;

}

Vector2 WrappingMove::getForwardVector() {
	return facingDirection;
}