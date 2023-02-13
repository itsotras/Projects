#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "HUD.h"
#include "Random.h"

Player::Player(Game* game) :Actor(game) {
	SetScale(scale);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Arwing.gpmesh"));
	mCollider = new CollisionComponent(this);
	mCollider->SetSize(width, height, depth);
	mMover = new PlayerMove(this);
	myHud = new HUD(this);
}

void Player::OnUpdate(float deltaTime) {
	cooldown -= deltaTime;

	if (cooldown <= 0.0f && shield < 3) {
		myHud->DoABarrelRoll();
		cooldown = Random::GetFloatRange(15.0f, 25.0f);
	}
}