#include "Checkpoint.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Player.h"
#include "HUD.h"

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
			Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Checkpoint.wav"), 0);
			if (!mLevelString.empty()) {
				mGame->SetNextLevelString(mLevelString);
			}
			if (!mText.empty()) {
				mGame->SetCheckptText(mText);
				mGame->GetPlayer()->GetComponent<HUD>()->UpdateCheckText();
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