#pragma once
#include "Actor.h"

class SecurityCamera : public Actor {
public:
	SecurityCamera(class Game* game, Actor* parent);
	~SecurityCamera();
	void OnUpdate(float deltaTime);
	void SetStartQ(Quaternion newQ) { startQ = newQ; }
	void SetEndQ(Quaternion newQ) { endQ = newQ; }
	void SetInterpTime(float time) { interpTime = time; }
	void SetPauseTime(float time) { pauseTime = time; }
	int GetVolume(float dist);
	void SetVolume(int channel);
private:
	Quaternion startQ = Quaternion::Identity;
	Quaternion endQ = Quaternion::Identity;
	float interpTime = 0.0f;
	float pauseTime = 0.0f;
	float lerpTimer = 0.0f;
	float pauseTimer = 0.0f;
	bool startToEnd = true;
	class SecurityCone* myCone;
	int myChannel = -1;
	bool firstTime = false;
	bool first = false;
};