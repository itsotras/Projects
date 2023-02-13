#include "Vehicle.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "WrappingMove.h"
#include "Game.h"
#include "Frog.h"
#include <string>

Vehicle::Vehicle(Game* game, char type, int line) :Actor(game) {

	mSprite = new SpriteComponent(this);
	std::string myType;
	float width = 64.0f;
	float height = 64.0f;
	if (type == 'A') {
		myType = "Assets/CarA.png";
	}
	else if(type == 'B') {
		myType = "Assets/CarB.png";
	}
	else if (type == 'C') {
		myType = "Assets/CarC.png";
	}
	else if (type == 'D') {
		myType = "Assets/CarD.png";
	}
	else if (type == 'T') {
		myType = "Assets/Truck.png";
		width = 128.0f;
		height = 48.0f;
	}
	mSprite->SetTexture(game->GetTexture(myType));

	mWrapper = new WrappingMove(this);
	mWrapper->SetForwardSpeed(100.0f);
	Vector2 myDirection;
	if (line % 2 == 0) {
		myDirection = Vector2(1.0f, 0.0f);
	}
	else {
		myDirection = Vector2(-1.0f, 0.0f);
	}
	mWrapper->setDirection(myDirection);

	mCollider = new CollisionComponent(this);
	mCollider->SetSize(width, height);

	mGame->AddVehicle(this);
}

Vehicle::~Vehicle() {
	mGame->RemoveVehicle(this);
}

void Vehicle::OnUpdate(float deltaTime) {

	Vector2 myPos = GetPosition();
	Vector2 forwardVector = GetComponent<WrappingMove>()->getForwardVector();//get the forward vector
	Frog* myFrog = mGame->getFrog();

	Vector2 frogPos = myFrog->GetPosition();
	Vector2 frogToVehicle = frogPos - myPos;


	float myAngle = Math::Acos(Vector2::Dot(forwardVector, frogToVehicle)/ frogToVehicle.Length());

	if (myAngle < (Math::Pi)/6.0f) {
		GetComponent<WrappingMove>()->SetForwardSpeed(mForwardSpeed / 2.0f);
	}
	else {
		GetComponent<WrappingMove>()->SetForwardSpeed(mForwardSpeed);
	}

}