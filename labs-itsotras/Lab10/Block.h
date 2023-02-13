#pragma once
#include "Actor.h"

class Block : public Actor {
public:
	Block(class Game* game);
	~Block();
	void SetMirror(bool mirror) { isMirror = mirror; }
	bool GetMirror() { return isMirror; }
private:
	float scale = 64.0f;
	class MeshComponent* mMesh;
	class CollisionComponent* mCollider;
	bool isMirror = false;
};