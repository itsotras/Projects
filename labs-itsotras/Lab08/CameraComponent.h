#pragma once
#include "Component.h"
#include "Math.h"

class CameraComponent : public Component {
public:
	CameraComponent(Actor* owner);
	void Update(float deltaTime);
	class Vector3 GetIdealPosition();
	void SnapToIdeal();

private:
	float hDist = 60.0f;
	float targetDist = 50.0f;

	float springConstant = 256.0f;
	float dampConstant = 2.0f * Math::Sqrt(springConstant);

	Vector3 cameraPos = GetIdealPosition();
	Vector3 cameraVelocity = Vector3::Zero;
};