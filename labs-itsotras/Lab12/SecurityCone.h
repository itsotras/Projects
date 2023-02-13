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
	float insideTimer = 2.0f;
	float deathTimer = 0.5f;
	bool isCaught = false;
	int detectedChannel = -1;

};