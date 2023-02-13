#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	Vector2 myMaxPos = GetMax();
	Vector2 myMinPos = GetMin();
	Vector2 otherMaxPos = other->GetMax();
	Vector2 otherMinPos = other->GetMin();

	bool case1 = myMaxPos.x < otherMinPos.x;//check if I am to the left of other
	bool case2 = myMinPos.x > otherMaxPos.x;//check if I am to the right of other
	bool case3 = myMinPos.y > otherMaxPos.y;//check if I am below other
	bool case4 = myMaxPos.y < otherMinPos.y;//check if I am above other

	if (case1 || case2 || case3 || case4) {//if any of these are true, that means I am not intersecting
		return false;
	}

	return true;//otherwise I must be intersecting with these
}

Vector2 CollisionComponent::GetMin() const
{
	Vector2 min;
	Vector2 myPos = mOwner->GetPosition();
	float ownerX = myPos.x;
	float ownerY = myPos.y;
	float ownerScale = mOwner->GetScale();

	min.x = ownerX - (mWidth * ownerScale) / 2.0f;
	min.y = ownerY - (mHeight * ownerScale) / 2.0f;
	return min;
}

Vector2 CollisionComponent::GetMax() const
{
	Vector2 max;
	Vector2 myPos = mOwner->GetPosition();
	float ownerX = myPos.x;
	float ownerY = myPos.y;
	float ownerScale = mOwner->GetScale();

	max.x = ownerX + (mWidth * ownerScale) / 2.0f;
	max.y = ownerY + (mHeight * ownerScale) / 2.0f;
	return max;
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector2& offset)
{
	offset = Vector2::Zero;

	if (!this->Intersect(other)) {
		return CollSide::None;
	}
	else {
		Vector2 myMaxPos = GetMax();
		Vector2 myMinPos = GetMin();
		Vector2 otherMaxPos = other->GetMax();
		Vector2 otherMinPos = other->GetMin();

		float topDist = myMaxPos.y - otherMinPos.y ;
		float botDist = otherMaxPos.y - myMinPos.y;
		float rightDist = otherMaxPos.x - myMinPos.x;
		float leftDist = myMaxPos.x - otherMinPos.x ;

		float min1 = Math::Min(abs(topDist), abs(botDist));
		float min2 = Math::Min(abs(rightDist), abs(leftDist));
		float min3 = Math::Min(min1, min2);

		if (min3 == abs(topDist)) {

			offset.y -= topDist;
			return CollSide::Top;
		}
		else if (min3 == abs(botDist)) {
			offset.y += botDist;

			return CollSide::Bottom;
		}
		else if (min3 == abs(leftDist)) {
			offset.x -= leftDist;
			return CollSide::Left;
		}
		else {
			offset.x += rightDist;
			return CollSide::Right;
		}
	}
}
