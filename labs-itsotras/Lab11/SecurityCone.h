#pragma once
#include "Actor.h"

class SecurityCone : public Actor {
public:
	SecurityCone(class Game* game, Actor* parent);
	void OnUpdate(float deltaTime);
	bool IsCaught() { return isCaught; }
private:
	class MeshComponent* mMesh;
	float mScale = 3.0f;
	float height = 100.0f * mScale;
	bool isCaught = false;
	const float expected_angle = (Math::Pi / 6.0f);
	float insideTime = 2.0f;
	float deathTime = 0.5f;
	float insideTimer = insideTime;
	float deathTimer = deathTime;

};