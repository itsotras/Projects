#include "SecurityCone.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Player.h"
#include "PlayerMove.h"
#include "SecurityCamera.h"

SecurityCone::SecurityCone(Game* game, Actor* parent): Actor(game, parent) {
	mMesh = new MeshComponent(this, true);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cone.gpmesh"));
	SetPosition(Vector3(30.0f, 0.0f, 0.0f));
	SetScale(mScale);
}

void SecurityCone::OnUpdate(float deltaTime) {
	Vector3 ConeToPlayer = mGame->GetPlayer()->GetPosition() - GetWorldPosition();//get the vector from Security Camera to 
	float angle = Math::Acos(Vector3::Dot(GetWorldForward(), ConeToPlayer)/ConeToPlayer.Length());
	float distanceToPlayer = Math::Abs(Vector3::Dot(GetWorldForward(), ConeToPlayer));
	bool angleBool = (angle > (Math::Pi / 6.0f));
	if (distanceToPlayer > height || angleBool){
		if (isCaught) {
			if (detectedChannel != -1) {
				Mix_HaltChannel(detectedChannel);
			}
			isCaught = false;
		}
		mMesh->SetTextureIndex(0);//white
	}
	else {//if the player is inside the cone
		if (!isCaught) {
			detectedChannel = Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/SecurityDetected.wav"), 0);
			deathTimer = .5f;
			insideTimer = 2.0f;
			isCaught = true;
		}
		mMesh->SetTextureIndex(1);//yellow
		if (insideTimer > 0.0f) {
			insideTimer -= deltaTime;
		}
		else {
			mMesh->SetTextureIndex(2);//red
			if (deathTimer > 0.0f) {
				deathTimer -= deltaTime;
			}
			else {
				mGame->GetPlayer()->GetComponent<PlayerMove>()->Respawn();
				deathTimer = .5f;
				insideTimer = 2.0f;
			}
		}
	}
}