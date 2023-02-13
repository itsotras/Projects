#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "Random.h"
#include "CameraComponent.h"
#include "PlayerUI.h"

Player::Player(Game* game) :Actor(game) {
	
	SetScale(scale);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
	mMover = new PlayerMove(this);
	CameraComponent* camera = new CameraComponent(this);
	camera->SnapToIdeal();
	myUI = new PlayerUI(this);
}

void Player::OnUpdate(float deltaTime) {

}