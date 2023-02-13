#pragma once
#include "Actor.h"
class Ship : public Actor{
public:
	Ship(Game* game);
	

private:
	void OnProcessInput(const Uint8* keyState);
	void OnUpdate(float deltaTime);
	class MoveComponent* mMove;
	class SpriteComponent* mSprite;
	float coolDown = 0.0f;
	float shipForwardSpeed = 300.0f;
	float shipRotateSpeed = 10.0f;
};