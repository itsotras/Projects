#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
,mDepth(0.0f)
{
}

CollisionComponent::~CollisionComponent()
{
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	Vector3 aMin = GetMin();
	Vector3 aMax = GetMax();
	Vector3 bMin = other->GetMin();
	Vector3 bMax = other->GetMax();

	bool no = aMax.x < bMin.x ||
		aMax.y < bMin.y ||
		aMax.z < bMin.z ||
		bMax.x < aMin.x ||
		bMax.y < aMin.y ||
		bMax.z < aMin.z;

	return !no;
}

Vector3 CollisionComponent::GetMin() const
{
	Vector3 v = mOwner->GetPosition();
	v.x -= mDepth * mOwner->GetScale() / 2.0f;
	v.y -= mWidth * mOwner->GetScale() / 2.0f;
	v.z -= mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetMax() const
{
	Vector3 v = mOwner->GetPosition();
	v.x += mDepth * mOwner->GetScale() / 2.0f;
	v.y += mWidth * mOwner->GetScale() / 2.0f;
	v.z += mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

const Vector3& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector3& offset)
{
	offset = Vector3::Zero;
	if (!this->Intersect(other)) {
		return CollSide::None;
	}
	else {
		Vector3 myMaxPos = GetMax(); //my maximum position
		Vector3 myMinPos = GetMin(); //my minimum position
		Vector3 otherMaxPos = other->GetMax();//the maximum position of other
		Vector3 otherMinPos = other->GetMin();//the minimum position of other

		float topDist = otherMaxPos.z - myMinPos.z; //difference between the maxZ of other and my minZ
		float bottomDist = myMaxPos.z - otherMinPos.z; //difference between the maxZ of me and other's minZ
		float rightDist = otherMaxPos.y - myMinPos.y; //difference between the maxY of other and my minY
		float leftDist = myMaxPos.y - otherMinPos.y; //difference between the maxY of me and other's minY
		float frontDist = otherMaxPos.x - myMinPos.x; //difference between the maxX of other and my minX
		float backDist = myMaxPos.x - otherMinPos.x; //difference between the maxX of me and other's minX

		float min1 = Math::Min(abs(topDist), abs(bottomDist));
		float min2 = Math::Min(abs(rightDist), abs(leftDist));
		float min3 = Math::Min(abs(frontDist), abs(backDist));
		float min4 = Math::Min(min1, min2);
		float min5 = Math::Min(min3, min4);//get the minimums of all the distances

		if (min5 == abs(topDist)) { //if the smallest absolute value is my topDist, I collided with the top of other 
			offset.z += topDist;
			return CollSide::Top;
		}
		else if (min5 == abs(bottomDist)) { //if the smallest absolute value is my bottomDist, I collided with the bottom of other 
			offset.z -= bottomDist;
			return CollSide::Bottom;
		}
		else if (min5 == abs(leftDist)) {//if the smallest absolute value is my leftDist, I collided with the left of other 
			offset.y -= leftDist;
			return CollSide::Left;
		}
		else if (min5 == abs(rightDist)) {//if the smallest absolute value is my rightDist, I collided with the right of other 
			offset.y += rightDist;
			return CollSide::Right;
		}
		else  if (min5 == abs(frontDist)) { //if the smallest absolute value is my frontDist, I collided with the front of other 
			offset.x += frontDist;
			return CollSide::Front;
		}
		else { //If the smallest absolute value is my backDist, I collided with the back of other
			offset.x -= backDist;
			return CollSide::Back;
		}
	}
}
