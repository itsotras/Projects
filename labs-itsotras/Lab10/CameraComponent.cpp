#include "CameraComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

CameraComponent::CameraComponent(Actor* owner) : Component(owner) {
	//SnapToIdeal();
}

void CameraComponent::Update(float deltaTime) {

	//calculate eyePos and targetPos
	Vector3 cameraPos = mOwner->GetPosition();

	mPitchAngle += mPitchSpeed * deltaTime;
	mPitchAngle = Math::Clamp(-Math::Pi / 4.0f, Math::Pi / 4.0f, mPitchAngle);

	Matrix4 pitch = Matrix4::CreateRotationY(mPitchAngle);
	Matrix4 yaw = Matrix4::CreateRotationZ(mOwner->GetRotation());

	Vector3 newVector = Vector3::Transform(Vector3(1.0f, 0.0f, 0.0f), pitch*yaw);

	Vector3 newUpVector = Vector3::UnitZ;
	Matrix4 rotation;

	if (onWall) {
		angle += -.50f * deltaTime;
		if (angle < -.20f) {
			angle = -.20f;
		}
		useNewUp = true;
	}
	else {
		if (angle < 0.0f) {
			angle += .50f * deltaTime;
			useNewUp = true;
		}
		else {
			useNewUp = false;
		}
	}

	if (mySide == CollSide::Left) {
		rotation = Matrix4::CreateRotationX(-1 * angle);
	}
	else if (mySide == CollSide::Right) {
		rotation = Matrix4::CreateRotationX(angle);
	}
	else if (mySide == CollSide::Front) {
		rotation = Matrix4::CreateRotationY(-1 * angle);
	}
	else if (mySide == CollSide::Back) {
		rotation = Matrix4::CreateRotationY(angle);
	}

	if (useNewUp) {
		newUpVector = Vector3::Transform(newUpVector, rotation);
	}
	

	Vector3 targetPos = mOwner->GetPosition() + (newVector) * targetDist;
	Matrix4 camera = Matrix4::CreateLookAt(cameraPos, targetPos, newUpVector);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(camera);

}
