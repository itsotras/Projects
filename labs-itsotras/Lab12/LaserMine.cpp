#include "LaserMine.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "LaserComponent.h"

LaserMine::LaserMine(Game* game, Actor* parent) :Actor(game, parent) {
	MeshComponent* mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/LaserMine.gpmesh"));
	mLaser = new LaserComponent(this);
}