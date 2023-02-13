#include "Coin.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Player.h"
#include "HUD.h"
Coin::Coin(Game* game, Actor* parent) : Actor(game, parent) {
	MeshComponent* mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Coin.gpmesh"));
	mCollider = new CollisionComponent(this);
	mCollider->SetSize(100.0f, 100.0f, 100.0f);
}

void Coin::OnUpdate(float deltaTime) {
	SetRotation(GetRotation() + Math::Pi*deltaTime);
	if (mCollider->Intersect(mGame->GetPlayer()->getCollider())) {
		Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Coin.wav"), 0);
		GetGame()->GetPlayer()->GetComponent<HUD>()->GetCoin();
		SetState(ActorState::Destroy);
	}
}