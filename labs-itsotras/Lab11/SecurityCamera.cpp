#include "SecurityCamera.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "SecurityCone.h"
#include "Player.h"

SecurityCamera::SecurityCamera(Game* game, Actor* parent) :Actor(game, parent) {
	MeshComponent* mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Camera.gpmesh"));
	myCone = new SecurityCone(game, this);
}

void SecurityCamera::OnUpdate(float deltaTime) {
	if (!myCone->IsCaught()) {
		if (lerpTimer == interpTime) {//I have stopped lerping
			if (pauseTimer >= pauseTime) {//I have stopped pausing, will start moving
				pauseTimer = 0.0f;
				lerpTimer = 0.0f;
				startToEnd = !startToEnd;
			}
			else {//I am paused
				pauseTimer += deltaTime;
			}
		}
		else {
			if (lerpTimer > interpTime) {
				lerpTimer = interpTime;
				//reach the end of my lerping
			}
			else {//increase lerpTimer
				lerpTimer += deltaTime;
			}
			Quaternion start;
			Quaternion end;
			if (startToEnd) {
				start = startQ;
				end = endQ;
			}
			else {
				start = endQ;
				end = startQ;
			}
			Quaternion newQ = Quaternion::Slerp(start, end, lerpTimer / interpTime);
			SetQuat(newQ);
		}
	}
}