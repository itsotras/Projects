#pragma once
#include "Actor.h"
#include <string>

class Checkpoint : public Actor {
public:
	Checkpoint(class Game* game, Actor* parent);
	void SetActive(bool active) { isActive = active; }
	bool GetActive() { return isActive; }
	void OnUpdate(float deltaTime);
	class CollisionComponent* GetCollider() { return mCollider; }
	void SetLevelString(std::string levelString) { mLevelString = levelString; }
	void SetTextString(std::string textString) { mText = textString; }
private:
	bool isActive = false;
	class MeshComponent* mMesh;
	class CollisionComponent* mCollider;
	std::string mLevelString;
	std::string mText;
};