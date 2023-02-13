#include "Log.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "WrappingMove.h"
#include "Game.h"
#include <string>

Log::Log(Game* game, char type, int line) :Actor(game) {

	mSprite = new SpriteComponent(this);
	std::string myType;
	float width = 0.0f;
	if (type == 'X') {
		myType = "Assets/LogX.png";
		width = 192.0f;
	}
	else if (type == 'Y') {
		myType = "Assets/LogY.png";
		width = 256.0f;
	}
	else if (type == 'Z') {
		myType = "Assets/LogZ.png";
		width = 384.0f;
	}
	mCollider = new CollisionComponent(this);
	mCollider->SetSize(width, 48.0f);

	mWrapper = new WrappingMove(this);
	mWrapper->SetForwardSpeed(75.0f);

	Vector2 myDirection;

	if (line % 2 == 0) {
		myDirection = Vector2(1.0f, 0.0f);
	}
	else {
		myDirection = Vector2(-1.0f, 0.0f);
	}
	mWrapper->setDirection(myDirection);
	
	mSprite->SetTexture(game->GetTexture(myType));
	mGame->AddLog(this);
}

Log::~Log() {
	mGame->RemoveLog(this);
}