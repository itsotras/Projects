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

	Vector2 pos = mOwner->GetPosition();

	Vector2 velocity = mOwner->GetForward(angle) * mForwardSpeed;//calculate the velocity

	pos += velocity * deltaTime;//set position

	mOwner->SetPosition(pos);

}


