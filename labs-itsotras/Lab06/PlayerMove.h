#pragma once
#include "MoveComponent.h"
#include "Math.h"
#include "SDL2/SDL_mixer.h"
enum class MoveState {
	Left,
	Right,
	Up,
	Down,
	None
};

class PlayerMove : public MoveComponent {

public:
	PlayerMove(class Actor* owner);
	~PlayerMove();
	void ProcessInput(const Uint8* keyState);
	void Update(float deltaTime);
	void SetMyAnimationSword();
	void SetMyAnimations(float deltaTime, Vector2& myPos, class AnimatedSprite* myAnimator);

private:
	float speed = 150.0f;
	float attackTimer = .25f;
	MoveState mMoveState = MoveState::Down;
	Vector2 myDirection = Vector2(0.0f, 1.0f);
	bool spacePressed = false;
	class Sword* mySword;
};
