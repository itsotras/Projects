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

	Vector3 targetPos = mOwner->GetPosition() + (newVector) * targetDist;
	Matrix4 camera = Matrix4::CreateLookAt(cameraPos, targetPos, Vector3::UnitZ);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(camera);

}
