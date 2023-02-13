#include "Block.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include <string>
#include "Game.h"

Block::Block(Game* game, char type) : Actor(game) {

	mSprite = new SpriteComponent(this);

	mCollider = new CollisionComponent(this);
	mCollider->SetSize(blockSize, blockSize);

	std::string myType = "Assets/Block";
	myType += type;
	myType += ".png";
	
	mSprite->SetTexture(mGame->GetTexture(myType));
	mGame->AddBlock(this);
}

Block::~Block() {
	mGame->RemoveBlock(this);
}

CollisionComponent* Block::GetCollider() {
	return mCollider;
}