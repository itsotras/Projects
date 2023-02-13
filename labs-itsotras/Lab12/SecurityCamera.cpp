#include "SecurityCamera.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "SecurityCone.h"
#include "Player.h"

SecurityCamera::SecurityCamera(Game* game, Actor* parent):Actor(game, parent) {
	MeshComponent* mMesh = new MeshComponent(this);
	mMesh->SetMesh(mGame->GetRenderer()->GetMesh("Assets/Camera.gpmesh"));
	myCone = new SecurityCone(game, this);
	mGame->GetCameras().push_back(this);
}

SecurityCamera::~SecurityCamera() {
	if (Mix_Playing(myChannel) != 0) {
		Mix_HaltChannel(myChannel);
	}
	auto iter = std::find(mGame->GetCameras().begin(), mGame->GetCameras().end(), this);
	if (iter != mGame->GetCameras().end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mGame->GetCameras().end() - 1;
		std::iter_swap(iter, iter2);
		mGame->GetCameras().pop_back();
	}
}

void SecurityCamera::OnUpdate(float deltaTime){
	int volume = GetVolume((GetPosition() - mGame->GetPlayer()->GetPosition()).Length());
	if (!first) {
		myChannel = Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/CameraMotor.wav"), 0);
		SetVolume(myChannel);
		first = true;//when the sound starts playing first
	}
	if (!myCone->IsCaught()) {
		if (firstTime) {
			myChannel = Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/CameraMotor.wav"), 0);
		}
		firstTime = false;
		if (lerpTimer == interpTime) {//I have stopped lerping
			if (pauseTimer >= pauseTime) {//I have stopped pausing, will start moving
				pauseTimer = 0.0f;
				lerpTimer = 0.0f;
				startToEnd = !startToEnd;
				myChannel = Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/CameraMotor.wav"), 0);
			}
			else {//I am paused
				pauseTimer += deltaTime;
			}
		}
		else {
			if (lerpTimer > interpTime) {
				lerpTimer = interpTime;
				//reach the end of my lerping
				Mix_HaltChannel(myChannel);
				Mix_Volume(Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/CameraMotorStop.wav"), 0), volume);
			}
			else {//increase lerpTimer
				SetVolume(myChannel);
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
	else {
		if (!firstTime) {
			Mix_HaltChannel(myChannel);
			Mix_Volume(Mix_PlayChannel(Mix_GroupAvailable(1), GetGame()->GetSound("Assets/Sounds/CameraMotorStop.wav"), 0), volume);
			firstTime = true;
		}
	}
}

int SecurityCamera::GetVolume(float dist) {
	int value = (int)((-128.0f / 1000.0f) * dist + 194);
	return Math::Clamp(0, 128, value);
}

void SecurityCamera::SetVolume(int channel) {
	float dist = (GetPosition() - mGame->GetPlayer()->GetPosition()).Length();
	if (dist > 1500.0f) {
		if (channel != -1) {
			Mix_Volume(channel, 0);
		}
	}
	else if (dist <= 500.0f) {
		if (channel != -1) {
			Mix_Volume(channel, 128);
		}
	}
	else {
		if (myChannel != -1) {
			Mix_Volume(channel, GetVolume(dist));
		}
	}
}