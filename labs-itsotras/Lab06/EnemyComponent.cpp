#include "EnemyComponent.h"
#include "Actor.h"
#include "CollisionComponent.h"
#include "Game.h"

EnemyComponent::EnemyComponent(Actor* owner): Component(owner) {
	mOwner->GetGame()->getEnemyComps().push_back(this);
	mCollider = mOwner->GetComponent<CollisionComponent>();
	SetOnDeath([this] {
		mOwner->SetState(ActorState::Destroy);
		});
}
EnemyComponent::~EnemyComponent(){
	std::vector<EnemyComponent*>::iterator it = std::find(mOwner->GetGame()->getEnemyComps().begin(), mOwner->GetGame()->getEnemyComps().end(), this);
	mOwner->GetGame()->getEnemyComps().erase(it);//erase the enemyComponent
}
CollisionComponent* EnemyComponent::getCollider() {
	return mCollider;
}
int EnemyComponent::getHP() {
	return hP;
}
void EnemyComponent::setHP(int newHP) {
	hP = newHP;
}
void EnemyComponent::TakeDamage() {
	if (timestamp2 - timestamp1 > damageTimer) {
		hP -= 1;
		if (hP == 0) {
			if (mOnDeath) {
				mOnDeath();
			}
		}
		else {
			if (mOnDamage) {
				mOnDamage();
			}
		}
		timestamp1 = timestamp2;
	}
}
void EnemyComponent::Update(float deltaTime) {
	timestamp2 += deltaTime;
	
}