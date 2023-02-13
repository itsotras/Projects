#include "Player.h"
#include "SpriteComponent.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "Game.h"

Player::Player(Game* game) :Actor(game) {

	mCollider = new CollisionComponent(this);
	mCollider->SetSize(playerSize, playerSize);

	mMover = new PlayerMove(this);
	mAnimated = new AnimatedSprite(this);
	mAnimated->LoadAnimations("Assets/Link");
	mAnimated->SetAnimation("StandDown");
}

Player::~Player() {

}

void Player::OnUpdate(float deltaTime) {
	
}


AnimatedSprite* Player::getAnimator() {
	return mAnimated;
}