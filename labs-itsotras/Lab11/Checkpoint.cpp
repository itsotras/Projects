#include "Checkpoint.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Player.h"

Checkpoint::Checkpoint(Game* game,Actor* parent):Actor(game, parent) {
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Checkpoint.gpmesh"));
	mCollider = new CollisionComponent(this);
	mCollider->SetSize(25.0f, 25.0f, 25.0f);
	
}

void Checkpoint::OnUpdate(float deltaTime) {
	if (isActive) {
		mMesh->SetTextureIndex(0);
		CollisionComponent* playerCol = mGame->GetPlayer()->getCollider();
		if (mCollider->Intersect(playerCol)) {
			if (!mLevelString.empty()) {
				mGame->SetNextLevelString(mLevelString);
			}
			mGame->GetCheckpts().pop();
			mGame->GetPlayer()->SetRespawnPos(GetPosition());
			SetActive(false);
			SetState(ActorState::Destroy);
		}
	}
	else {
		mMesh->SetTextureIndex(1);
	}

		
}