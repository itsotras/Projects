#pragma once
#include "Actor.h"

class Block : public Actor {
public:
	Block(class Game* game, Actor* parent);
	~Block();
	void SetMirror(bool mirror) { isMirror = mirror; }
	bool GetMirror() { return isMirror; }
	void SetRotate(bool rotate) { isRotate = rotate; }
	bool GetRotate() { return isRotate; }
	void OnUpdate(float deltaTime);
private:
	float scale = 64.0f;
	class MeshComponent* mMesh;
	class CollisionComponent* mCollider;
	bool isMirror = false;
	bool isRotate = false;
};