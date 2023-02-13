#include "Collider.h"
#include "CollisionComponent.h"

Collider::Collider(Game* game, float width, float height): Actor(game) {
	mCollision = new CollisionComponent(this);
	mCollision->SetSize(width, height);
}

CollisionComponent* Collider::GetCollisionComp() {
	return mCollision;
}