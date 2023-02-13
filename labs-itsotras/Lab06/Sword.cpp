#include "Sword.h"
#include "CollisionComponent.h"
#include "Game.h"

Sword::Sword(Game* owner) :Actor(owner) {
	mCollider = new CollisionComponent(this);
	mCollider->SetSize(big_size, big_size);
}
void Sword::changeSize(float width, float height, Vector2 offset) {
	mCollider->SetSize(width, height);
	SetPosition(offset);
}

CollisionComponent* Sword::getMyCollider() {
	return mCollider;
}