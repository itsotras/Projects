#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "Random.h"
#include "CameraComponent.h"
#include "Arrow.h"

Player::Player(Game* game, Actor* parent) :Actor(game, parent) {
	
	mMover = new PlayerMove(this);
	CameraComponent* camera = new CameraComponent(this);
	camera->GetPitchSpeed();
	mCollider = new CollisionComponent(this);
	mCollider->SetSize(width, height, depth);
	
}

void Player::OnUpdate(float deltaTime) {

}