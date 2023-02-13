#pragma once
#include "Component.h"
#include "Math.h"

class CameraComponent : public Component {
public:
	CameraComponent(Actor* owner);
	void Update(float deltaTime);
	void SetPitchSpeed(float newSpeed) { mPitchSpeed = newSpeed; }
	float GetPitchSpeed() { return mPitchSpeed; }

private:
	
	float targetDist = 50.0f;
	float mPitchAngle = 0.0f;
	float mPitchSpeed = 0.0f;

};