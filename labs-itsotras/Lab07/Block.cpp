#include "Block.h"
#include "Game.h"
#include "Mesh.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"
#include "Player.h"

Block::Block(Game* game, size_t index):Actor(game) {
	SetScale(25.0f);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	mc->SetTextureIndex(index);
	if (index == 4) {
		isExploding = true;
	}
	mCollider = new CollisionComponent(this);
	mCollider->SetSize(1.0f, 1.0f, 1.0f);
	mGame->GetBlocks().push_back(this);
}

Block::~Block() {

	auto iter = std::find(mGame->GetBlocks().begin(), mGame->GetBlocks().end(), this);
	if (iter != mGame->GetBlocks().end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mGame->GetBlocks().end() - 1;
		std::iter_swap(iter, iter2);
		mGame->GetBlocks().pop_back();
	}

}

void Block::OnUpdate(float deltaTime) {
	if (mGame->GetPlayer()->GetPosition().x - GetPosition().x >= 2000.0f) {
		mState = ActorState::Destroy;
	}
}

void Block::Explode() {
	if (mState == ActorState::Destroy) {//if the block is already set to be destroyed
		return;
	}
	else {
		mState = ActorState::Destroy;//set this block to be destroyed
		if (isExploding) {//only if this block is an exploding block
			for (auto block: mGame->GetBlocks()) {
				if (block != this) {
					if (Vector3::Distance(block->GetPosition(), GetPosition())<=50.0f) {
						block->Explode();
					}
				}
			}
		}
	}
}