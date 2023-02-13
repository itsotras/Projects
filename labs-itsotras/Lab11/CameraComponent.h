#pragma once
#include "Component.h"
#include "CollisionComponent.h"
#include "Math.h"

class CameraComponent : public Component {
public:
	CameraComponent(Actor* owner);
	void Update(float deltaTime);
	void SetPitchSpeed(float newSpeed) { mPitchSpeed = newSpeed; }
	float GetPitchSpeed() { return mPitchSpeed; }
	void SetOnWall(bool status) { onWall = status; }
	void SetSide(CollSide side) { mySide = side; }

private:
	bool onWall = false;
	CollSide mySide = CollSide::None;
	float targetDist = 50.0f;
	float mPitchAngle = 0.0f;
	float mPitchSpeed = 0.0f;
	float angle = 0.0f;

};