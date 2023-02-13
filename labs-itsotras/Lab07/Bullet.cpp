#include "Bullet.h"
#include "CollisionComponent.h"
#include "MoveComponent.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Game.h"
#include "Block.h"
#include "SDL2/SDL_mixer.h"

Bullet::Bullet(class Game* game, float speedMultiplier):Actor(game) {
	SetScale(5.0f);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Laser.gpmesh"));
	MoveComponent* mMover = new MoveComponent(this);
	mMover->SetForwardSpeed(900.0f*speedMultiplier);
	mCollider = new CollisionComponent(this);
	mCollider->SetSize(10.0f, 10.0f, 10.0f);
	
}

void Bullet::OnUpdate(float deltaTime) {	
	if (timer > 0.0f) {
		timer -= deltaTime;
	}
	else {
		mState = ActorState::Destroy;
	}
	int blocksHit = 0;
	for (auto& block : mGame->GetBlocks()) {
		CollisionComponent* blockCollider = block->GetCollider();
		if (mCollider->Intersect(blockCollider)){
			if (block->GetExploding()) {
				if (blocksHit == 0) {
					Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/ShipDie.wav"), 0);
				}
				blocksHit++;
				//start the chain Explosion
				block->Explode();
			}
			mState = ActorState::Destroy;
		}
	}
}

