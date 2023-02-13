#include "Player.h"
#include "SpriteComponent.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "Game.h"

Player::Player(Game* game) :Actor(game) {

	mAnimated = new AnimatedSprite(this);

	std::vector<SDL_Texture*> idleAnim{
	GetGame()->GetTexture("Assets/Mario/Idle.png")
	};
	mAnimated->AddAnimation("idle", idleAnim);

	std::vector<SDL_Texture*> deadAnim{
	GetGame()->GetTexture("Assets/Mario/Dead.png")
	};
	mAnimated->AddAnimation("dead", deadAnim);

	std::vector<SDL_Texture*> jumpLeftAnim{
	GetGame()->GetTexture("Assets/Mario/JumpLeft.png")
	};
	mAnimated->AddAnimation("jumpLeft", jumpLeftAnim);

	std::vector<SDL_Texture*> jumpRightAnim{
	GetGame()->GetTexture("Assets/Mario/JumpRight.png")
	};
	mAnimated->AddAnimation("jumpRight", jumpRightAnim);

	std::vector<SDL_Texture*> runRightAnim{
	GetGame()->GetTexture("Assets/Mario/RunRight0.png"),
	GetGame()->GetTexture("Assets/Mario/RunRight1.png"),
	GetGame()->GetTexture("Assets/Mario/RunRight2.png")
	};
	mAnimated->AddAnimation("runRight", runRightAnim);

	std::vector<SDL_Texture*> runLeftAnim{
	GetGame()->GetTexture("Assets/Mario/RunLeft0.png"),
	GetGame()->GetTexture("Assets/Mario/RunLeft1.png"),
	GetGame()->GetTexture("Assets/Mario/RunLeft2.png")
	};
	mAnimated->AddAnimation("runLeft", runLeftAnim);
	mAnimated->SetAnimation("idle");

	mCollider = new CollisionComponent(this);
	mCollider->SetSize(playerSize, playerSize);

	mMover = new PlayerMove(this);
}

Player::~Player() {

}

void Player::OnUpdate(float deltaTime) {
	if (isDead) {
		Mix_HaltChannel(mGame->getChannel());
		Mix_Chunk* myChunk = mGame->GetSound("Assets/Sounds/Dead.wav");
		Mix_PlayChannel(-1, myChunk, 0);
		SetState(ActorState::Paused);
		
	}
}

void Player::setDeath() {
	isDead = true;
}
bool Player::getDeath() {
	return isDead;
}

AnimatedSprite* Player::getAnimator() {
	return mAnimated;
}