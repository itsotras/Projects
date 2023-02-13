#include "Block.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"

Block::Block(Game* game) : Actor(game) {
	SetScale(scale);
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
	mCollider = new CollisionComponent(this);
	mCollider->SetSize(1.0f, 1.0f, 1.0f);
	mGame->getBlocks().emplace_back(this);
}

Block::~Block() {
	std::vector<Actor*> mBlocks = mGame->getBlocks();
	auto iter = std::find(mBlocks.begin(), mBlocks.end(), this);
	if (iter != mBlocks.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mBlocks.end() - 1;
		std::iter_swap(iter, iter2);
		mBlocks.pop_back();
	}
}