#include "Arrow.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Player.h"
#include "Checkpoint.h"

Arrow::Arrow(Game* game, Actor* parent):Actor(game, parent) {
	SetScale(.15f);
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Arrow.gpmesh"));
}

void Arrow::OnUpdate(float deltaTime) {
	Quaternion newQuat = Quaternion();
	if (mGame->GetCheckpts().empty()) {
		newQuat = Quaternion::Identity;
	}
	else {
		Vector3 direction = mGame->GetCheckpts().front()->GetPosition() - mGame->GetPlayer()->GetPosition();
		direction.Normalize();
		float dot = Vector3::Dot(Vector3::UnitX, direction);
		if (dot == 1.0f) {
			newQuat = Quaternion::Identity;
		}
		else if (dot == -1.0f) {
			newQuat = Quaternion(Vector3::UnitZ, Math::Pi);
		}
		else {
			float angle = Math::Acos(dot);
			Vector3 axis = Vector3::Cross(Vector3::UnitX, direction);
			axis = Vector3::Normalize(axis);
			newQuat = Quaternion(axis, angle);
		}
	}
	SetQuat(newQuat);
	Vector3 unProject = mGame->GetRenderer()->Unproject(Vector3(0.0f, 250.0f, 0.1f));
	SetPosition(unProject);
}