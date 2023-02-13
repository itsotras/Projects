#include "Frog.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Vehicle.h"
#include "CollisionComponent.h"
#include "DeadFrog.h"
#include "Log.h"
#include "WrappingMove.h"

Frog::Frog(Game* game):Actor(game) {
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(game->GetTexture("Assets/Frog.png"));
	
	lastFrames.insert(std::pair<SDL_Scancode, bool>(SDL_SCANCODE_UP, false));
	lastFrames.insert(std::pair<SDL_Scancode, bool>(SDL_SCANCODE_DOWN, false));
	lastFrames.insert(std::pair<SDL_Scancode, bool>(SDL_SCANCODE_LEFT, false));
	lastFrames.insert(std::pair<SDL_Scancode, bool>(SDL_SCANCODE_RIGHT, false));

	mCollider = new CollisionComponent(this);
	mCollider->SetSize(50.0f, 50.0f);

}

void Frog::OnProcessInput(const Uint8* keyState) {

	Vector2 myPos = GetPosition();

	if (keyState[SDL_SCANCODE_UP]) {//this frame is True
		if (lastFrames.at(SDL_SCANCODE_UP) == false) {//last Frame is false
			myPos.y -= 64.0f;
			lastFrames[SDL_SCANCODE_UP] = true;
		}
	}

	if (keyState[SDL_SCANCODE_DOWN]) {//this frame is True
		if (lastFrames.at(SDL_SCANCODE_DOWN) == false) {//last Frame is false
			myPos.y += 64.0f;
			lastFrames[SDL_SCANCODE_DOWN] = true;//set lastFrame to true
		}
	}

	if (keyState[SDL_SCANCODE_LEFT]) {//this frame is True
		if (lastFrames.at(SDL_SCANCODE_LEFT) == false) {//last Frame is false
			myPos.x -= 64.0f;
			lastFrames[SDL_SCANCODE_LEFT] = true;//set lastFrame to true
		}
	}

	if (keyState[SDL_SCANCODE_RIGHT]) {//this frame is True
		if (lastFrames.at(SDL_SCANCODE_RIGHT) == false) {//last Frame is false
			myPos.x += 64.0f;
			lastFrames[SDL_SCANCODE_RIGHT] = true;//set lastFrame to true
		}
	}

	lastFrames[SDL_SCANCODE_UP] = keyState[SDL_SCANCODE_UP];
	lastFrames[SDL_SCANCODE_DOWN] = keyState[SDL_SCANCODE_DOWN];
	lastFrames[SDL_SCANCODE_LEFT] = keyState[SDL_SCANCODE_LEFT];
	lastFrames[SDL_SCANCODE_RIGHT] = keyState[SDL_SCANCODE_RIGHT];

	float boundW = mGame->getWindowWidth();
	float boundH = mGame->getWindowHeight();

	myPos.x = Math::Clamp(myPos.x, 0.0f, boundW - 0.0f);
	myPos.y = Math::Clamp(myPos.y, 160.0f, boundH - 96.0f);

	this->SetPosition(myPos);
}

void Frog::OnUpdate(float deltaTime) {

	CollisionComponent* myCollider = GetComponent<CollisionComponent>();
	std::vector<Vehicle*>& myVehicles = mGame->getMyVehicles();
	for (auto vehicle : myVehicles) {
		if (myCollider->Intersect(vehicle->GetComponent<CollisionComponent>())) {
			DeadFrog* myGrave = new DeadFrog(mGame);//create a deadFrog
			myGrave->SetPosition(this->GetPosition());
			this->SetPosition(startPos);//reset my position
		}
	}
	std::vector<Log*>& myLogs = mGame->getMyLogs();
	onLog = false;
	for (auto log : myLogs) {
		Vector2 myPos = this->GetPosition();
		CollSide myStatus = myCollider->GetMinOverlap(log->GetComponent<CollisionComponent>(), myPos);
		if (myStatus == CollSide::None) {
			//do nothing
		}
		else {
			Vector2 moveIt;
			moveIt.y = log->GetPosition().y;
			moveIt.x = this->GetPosition().x;
			this->SetPosition(moveIt);

			WrappingMove* wrapper = log->GetComponent<WrappingMove>();
			Vector2 newVelocity = wrapper->getForwardVector()* wrapper->GetForwardSpeed()*deltaTime;
			moveIt += newVelocity;

			if (myStatus == CollSide::Left) {
				moveIt.x += myPos.x + 32.0f;
			}
			else if (myStatus == CollSide::Right) {
				moveIt.x += myPos.x - 32.0f;
			}

			this->SetPosition(moveIt);
			onLog = true;
		}
		
	}
	if (!onLog) {
		if (this->GetPosition().y >= 180 && this->GetPosition().y <=510) {
			DeadFrog* myGrave = new DeadFrog(mGame);//create a deadFrog
			myGrave->SetPosition(this->GetPosition());
			this->SetPosition(startPos);//reset my position
		}
	}

	Actor* theGoal = mGame->getGoal();
	CollisionComponent* goalCollider = theGoal->GetComponent<CollisionComponent>();
	if (myCollider->Intersect(goalCollider)) {
		this->SetPosition(theGoal->GetPosition());
		this->SetState(ActorState::Paused);
	}
	else {
		if (this->GetPosition().y <= 180) {
			DeadFrog* myGrave = new DeadFrog(mGame);//create a deadFrog
			myGrave->SetPosition(this->GetPosition());
			this->SetPosition(startPos);//reset my position
		}
	}
	
}