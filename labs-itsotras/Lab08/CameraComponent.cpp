#include "CameraComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"

CameraComponent::CameraComponent(Actor* owner) : Component(owner) {
	//SnapToIdeal();
}

void CameraComponent::Update(float deltaTime) {

	//calculate eyePos and targetPos
	Vector3 displacement = cameraPos - GetIdealPosition();
	Vector3 springAccel = (-springConstant * displacement) - (dampConstant * cameraVelocity);
	cameraVelocity += springAccel*deltaTime;
	cameraPos += cameraVelocity * deltaTime;
	Vector3 targetPos = mOwner->GetPosition() + (mOwner->GetForward(mOwner->GetRotation()) * targetDist);
	Matrix4 camera = Matrix4::CreateLookAt(cameraPos, targetPos, Vector3::UnitZ);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(camera);
}

Vector3 CameraComponent::GetIdealPosition() {
	Vector3 eyePos = mOwner->GetPosition() - (mOwner->GetForward(mOwner->GetRotation()) * hDist);
	eyePos.z = 70.0f;
	return eyePos;
}

void CameraComponent::SnapToIdeal() {
	Vector3 eyePos = GetIdealPosition();
	Vector3 targetPos = mOwner->GetPosition() + (mOwner->GetForward(mOwner->GetRotation()) * targetDist);
	Matrix4 camera = Matrix4::CreateLookAt(eyePos, targetPos, Vector3::UnitZ);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(camera);
}