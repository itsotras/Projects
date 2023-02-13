#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
:Component(owner, 50)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
{
	
}

void MoveComponent::Update(float deltaTime)
{	
	float angle = mOwner->GetRotation();//get rotation
	angle += mAngularSpeed * deltaTime;//change it
	mOwner->SetRotation(angle);

	Vector3 pos = mOwner->GetPosition();
	Vector3 velocity = mOwner->GetForward(angle) * mForwardSpeed;//calculate the velocity
	Vector3 sideVelocity = mOwner->GetRight(angle) * mStrafeSpeed;
	pos += (velocity + sideVelocity) * deltaTime;//set position
	//pos += sideVelocity * deltaTime;
	mOwner->SetPosition(pos);

}


