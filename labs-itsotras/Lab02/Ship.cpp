#include "Ship.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Laser.h"

Ship::Ship(Game* game)
:Actor(game) {

	mMove = new MoveComponent(this);
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(game->GetTexture("Assets/Ship.png"));

}

void Ship::OnProcessInput(const Uint8* keyState){
	
	mMove->SetForwardSpeed(0.0f);
	mMove->SetAngularSpeed(0.0f);
	float fSpeed = 0.0f;
	float aSpeed = 0.0f;
	mSprite->SetTexture(mGame->GetTexture("Assets/Ship.png"));

	if (keyState[SDL_SCANCODE_UP]) {//if the up key is pressed
		fSpeed += shipForwardSpeed;
	}

	if (keyState[SDL_SCANCODE_DOWN]) {//if the down key is pressed
		fSpeed -= shipForwardSpeed;
	}

	if (keyState[SDL_SCANCODE_LEFT]) {//if the left key is pressed
		aSpeed += shipRotateSpeed;
	}
	if (keyState[SDL_SCANCODE_RIGHT]) {//if the right key is pressed
		aSpeed -= shipRotateSpeed;
	}
	if (fSpeed != 0.0f) {
		mSprite->SetTexture(mGame->GetTexture("Assets/ShipThrust.png"));
	}

	if (keyState[SDL_SCANCODE_SPACE] && coolDown <= 0.0f) {//if the space key is pressed
		Laser* myLaser = new Laser(mGame);
		myLaser->SetPosition(this->GetPosition());
		myLaser->SetRotation(this->GetRotation());
		coolDown = 1.0f;
	}
	mMove->SetForwardSpeed(fSpeed);
	mMove->SetAngularSpeed(aSpeed);
}

void Ship::OnUpdate(float deltaTime) {
	coolDown -= deltaTime;
}