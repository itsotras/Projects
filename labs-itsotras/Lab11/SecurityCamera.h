#pragma once
#include "Actor.h"

class SecurityCamera : public Actor {
public:
	SecurityCamera(class Game* game, Actor* parent);
	void OnUpdate(float deltaTime);
	void SetStartQ(Quaternion newQ) { startQ = newQ; }
	void SetEndQ(Quaternion newQ) { endQ = newQ; }
	void SetInterpTime(float time) { interpTime = time; }
	void SetPauseTime(float time) { pauseTime = time; }
private:
	Quaternion startQ = Quaternion::Identity;
	Quaternion endQ = Quaternion::Identity;
	float interpTime = 0.0f;
	float pauseTime = 0.0f;
	float lerpTimer = 0.0f;
	float pauseTimer = 0.0f;
	bool startToEnd = true;
	class SecurityCone* myCone;
};