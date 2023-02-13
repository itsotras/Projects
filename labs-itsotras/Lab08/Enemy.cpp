#include "Enemy.h"
#include "EnemyMove.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"

Enemy::Enemy(Game* game) :Actor(game) {

	SetScale(scale);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh"));
	mc->SetTextureIndex(6);
	EnemyMove* mMove = new EnemyMove(this);
	mMove->GetUpdateOrder();
}