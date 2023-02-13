#pragma once
#include "Component.h"
#include <functional>

class EnemyComponent : public Component {
public:
	EnemyComponent(class Actor* owner);
	~EnemyComponent();
	class CollisionComponent* getCollider();
	int getHP();
	void setHP(int newHP);
	void Update(float deltaTime);
	void TakeDamage(); 
	void SetOnDeath(std::function<void()> OnDeath) { mOnDeath = OnDeath; };
	void SetOnDamage(std::function<void()> OnDamage) { mOnDamage = OnDamage; };
private:
	class CollisionComponent* mCollider;
	std::function<void()> mOnDeath;
	std::function<void()> mOnDamage;
	int hP = -1;
	float damageTimer = 0.25f;
	float timestamp1 = 0.0f;
	float timestamp2 = 0.0f;
};