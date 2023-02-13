#include "GoombaMove.h"
#include "Goomba.h"
#include "Math.h"
#include "Actor.h"
#include "CollisionComponent.h"
#include "Block.h"
#include "Game.h"

GoombaMove::GoombaMove(Actor* owner):MoveComponent(owner) {
	SetForwardSpeed(100.0f);
}
GoombaMove::~GoombaMove(){
}
void GoombaMove::Update(float deltaTime) {
	Vector2 pos = mOwner->GetPosition();
	pos.x += direction * mForwardSpeed * deltaTime;//set position
	pos.y += mYSpeed * deltaTime;
	if (pos.y > limit) {
		mOwner->SetState(ActorState::Destroy);
	}
	mOwner->SetPosition(pos);
	CollisionComponent* mCollider = mOwner->GetComponent<CollisionComponent>();
	const std::vector<Block*> blocks = mOwner->GetGame()->GetBlocks();
	for (auto block : blocks) {
		CollisionComponent* blockCollider = block->GetCollider();
		Vector2 offset;
		CollSide status = mCollider->GetMinOverlap(blockCollider, offset);
		if (status != CollSide::None) {
			if (status == CollSide::Top && mYSpeed > 0.0f) {
				mYSpeed = 0.0f;
				mOwner->SetPosition(mOwner->GetPosition() + offset);
			}

		}
	}
	for (auto block : blocks) {
		CollisionComponent* blockCollider = block->GetCollider();
		Vector2 offset;
		CollSide status = mCollider->GetMinOverlap(blockCollider, offset);
		if (status != CollSide::None) {
			if ((status == CollSide::Right || status == CollSide::Left) && block->GetPosition().y == mOwner->GetPosition().y) {
				direction *= -1;
				mOwner->SetPosition(mOwner->GetPosition()+offset);
			}
		}
	}
	const std::vector<Goomba*> enemies = mOwner->GetGame()->GetEnemies();
	for (auto goomba : enemies) {
		if (goomba == mOwner) {
			continue;
		}
		CollisionComponent* enemyCollider = goomba->GetCollider();
		Vector2 offset;
		CollSide status = mCollider->GetMinOverlap(enemyCollider, offset);
		if (status != CollSide::None) {
			if ((status == CollSide::Right || status == CollSide::Left)) {
				direction *= -1;
				mOwner->SetPosition(mOwner->GetPosition()+ offset);
			}
		}
	}
	mYSpeed += gravity * deltaTime;
}
