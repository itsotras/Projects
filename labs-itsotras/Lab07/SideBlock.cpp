#include "SideBlock.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Player.h"

SideBlock::SideBlock(Game* game, size_t index):Actor(game) {
	SetScale(scale);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	mc->SetTextureIndex(index);
}

void SideBlock::OnUpdate(float deltaTime) {
	if (mGame->GetPlayer()->GetPosition().x - GetPosition().x >= 2000.0f) {
		mState = ActorState::Destroy;
	}
}