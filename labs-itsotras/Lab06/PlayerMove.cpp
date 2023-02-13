#include "PlayerMove.h"
#include "Actor.h"
#include "Player.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include <SDL2/SDL_scancode.h>
#include "Collider.h"
#include "EnemyComponent.h"
#include "Sword.h"
//#include "SDL2/SDL_mixer.h"

PlayerMove::PlayerMove(Actor* owner) :MoveComponent(owner) {

	//mOwner->SetRotation(3*Math::PiOver2);
	mForwardSpeed = speed;
	mySword = new Sword(mOwner->GetGame());
}
PlayerMove::~PlayerMove() {
}
void PlayerMove::ProcessInput(const Uint8* keyState) {
	if (keyState[SDL_SCANCODE_LEFT]) {
		mMoveState = MoveState::Left;
		SetForwardSpeed(-1 * speed);

		myDirection = Vector2(-1.0f, 0.0f);
	}
	else if (keyState[SDL_SCANCODE_RIGHT]) {
		mMoveState = MoveState::Right;
		SetForwardSpeed(speed);
		myDirection = Vector2(1.0f, 0.0f);
	}
	else if (keyState[SDL_SCANCODE_UP]) {
		mMoveState = MoveState::Up;
		SetForwardSpeed(-1 * speed);
		myDirection = Vector2(0.0f, -1.0f);
	}
	else if (keyState[SDL_SCANCODE_DOWN]) {
		mMoveState = MoveState::Down;
		SetForwardSpeed(speed);
		myDirection = Vector2(0.0f, 1.0f);
	}
	else {
		mMoveState = MoveState::None;
		SetForwardSpeed(0.0f);
	}
	if (keyState[SDL_SCANCODE_SPACE] && attackTimer <= 0.0f) {
		if (!spacePressed) {
			Mix_Chunk* myChunk = mOwner->GetGame()->GetSound("Assets/Sounds/SwordSlash.wav");
			Mix_PlayChannel(-1, myChunk, 0);
			spacePressed = true;//set lastFrame to true
			attackTimer = .25f;
			SetMyAnimationSword();
		}
	}
	spacePressed = keyState[SDL_SCANCODE_SPACE];
}
void PlayerMove::Update(float deltaTime) {
	mOwner->GetGame()->GetCameraPos() = mOwner->GetPosition() + Vector2(-256.0f, -224.0f);
	Vector2 myPos = mOwner->GetPosition();
	AnimatedSprite* myAnimator = mOwner->GetGame()->getPlayer()->getAnimator();
	CollisionComponent* mCollider = mOwner->GetComponent<CollisionComponent>();
	CollisionComponent* swordCollider = mySword->getMyCollider();
	if (attackTimer > 0.0f) {//attack is going through
		attackTimer -= deltaTime;

	}
	else {
		SetMyAnimations(deltaTime, myPos, myAnimator);
	}
	mOwner->SetPosition(myPos);
	const std::vector<EnemyComponent*> enemyColliders = mOwner->GetGame()->getEnemyComps();
	//enemy checks
	for (auto collider : enemyColliders) {
		CollisionComponent* colliderCollision = collider->getCollider();
		Vector2 offset;
		if (attackTimer > 0.0f) {
			bool intersects = swordCollider->Intersect(colliderCollision);
			if (intersects) {
				collider->TakeDamage();
			}
		}
		CollSide status = mCollider->GetMinOverlap(colliderCollision, offset);
		if (status != CollSide::None) {
			mOwner->SetPosition(mOwner->GetPosition() + offset);
		}
	}
	//collider checks
	const std::vector<Collider*> colliders = mOwner->GetGame()->getColliders();
	for (auto collider: colliders) {
		CollisionComponent* colliderCollision = collider->GetCollisionComp();
		Vector2 offset;
		CollSide status = mCollider->GetMinOverlap(colliderCollision, offset);
		if (status != CollSide::None) {
			mOwner->SetPosition(mOwner->GetPosition() + offset);
		}
	}
}

void PlayerMove::SetMyAnimationSword() {
	AnimatedSprite* myAnimator = mOwner->GetGame()->getPlayer()->getAnimator();
	myAnimator->ResetAnimTimer();
	float smallOffset = 32.0f;
	float bigOffset = 40.0f;
	float big_size = 28.0f;
	float small_size = 20.0f;
	Vector2 offset = mOwner->GetPosition();
	float width = 0.0f;
	float height = 0.0f;
	if (myDirection.x == -1.0f) {
		myAnimator->SetAnimation("AttackLeft");
		width = small_size;
		height = big_size;
		offset.x -= smallOffset;
	}
	else if (myDirection.x == 1.0f) {
		myAnimator->SetAnimation("AttackRight");
		width = small_size;
		height = big_size;
		offset.x += smallOffset;
	}
	else if (myDirection.y == -1.0f) {
		myAnimator->SetAnimation("AttackUp");
		width = big_size;
		height = small_size;
		offset.y -= bigOffset;
	}
	else if (myDirection.y == 1.0f) {
		myAnimator->SetAnimation("AttackDown");
		width = big_size;
		height = small_size;
		offset.y += bigOffset;
	}
	mySword->changeSize(width, height, offset);
}

void PlayerMove::SetMyAnimations(float deltaTime, Vector2& myPos, AnimatedSprite* myAnimator) {
	if (mMoveState == MoveState::Left || mMoveState == MoveState::Right) {
		if (mMoveState == MoveState::Left) {
			myAnimator->SetAnimation("WalkLeft");
		}
		else {
			myAnimator->SetAnimation("WalkRight");
		}
		myPos.x += mForwardSpeed * deltaTime;
	}
	else if (mMoveState == MoveState::Up || mMoveState == MoveState::Down) {
		if (mMoveState == MoveState::Up) {
			myAnimator->SetAnimation("WalkUp");
		}
		else {
			myAnimator->SetAnimation("WalkDown");
		}
		myPos.y += mForwardSpeed * deltaTime;
	}
	else {
		if (myDirection.x == -1.0f) {
			myAnimator->SetAnimation("StandLeft");
		}
		else if (myDirection.x == 1.0f) {
			myAnimator->SetAnimation("StandRight");
		}
		else if (myDirection.y == -1.0f) {
			myAnimator->SetAnimation("StandUp");
		}
		else{
			myAnimator->SetAnimation("StandDown");
		}
	}
}