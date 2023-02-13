#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
#include <cmath>

Actor::Actor(Game* game, Actor* parent)
	:mGame(game)
	,mState(ActorState::Active)
	,mPosition(Vector3::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
{
	if (parent == nullptr) {
		mGame->AddActor(this);//adds actor
	}
	else {
		mParent = parent;
		mParent->AddChild(this);
	}
}
Actor::~Actor()
{
	while (mChildren.size()!= 0) {
		delete mChildren.back();
	}
	if (mParent == nullptr) {
		mGame->RemoveActor(this);//adds actor
	}
	else {
		mParent->RemoveChild(this);
	}
	for (int i = 0; i <(int)(mComponents.size()); i++) {
		delete mComponents[i];//delete the pointers
	}
	mComponents.clear();//clear the vector of components
}
void Actor::Update(float deltaTime)
{
	CalcWorldTransform();
	if (mState == ActorState::Active) {
		for (int i = 0; i < (int)(mComponents.size()); i++) {
			mComponents[i]->Update(deltaTime);//update the component at index i
		}
		this->OnUpdate(deltaTime);
	}
	CalcWorldTransform();
	for (int i = 0; i < (int)mChildren.size(); i++) {
		mChildren[i]->Update(deltaTime);
	}
}
void Actor::OnUpdate(float deltaTime)
{
}
void Actor::ProcessInput(const Uint8* keyState)
{
	if (mState == ActorState::Active) {
		for (int i = 0; i < (int)(mComponents.size()); i++) {
			mComponents[i]->ProcessInput(keyState);//process input on the component at index i
		}
		this->OnProcessInput(keyState);
	}
}
void Actor::OnProcessInput(const Uint8* keyState)
{
}
void Actor::AddComponent(Component* c)//adds a component
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}
Vector3 Actor::GetForward() {
	float radians = GetRotation();
	float x = (float)(cos(radians));//calculate the x component
	float y = (float)(sin(radians));//calculate the y component, y is pointing down
	Vector3 v1(x, y, 0.0f);
	return v1;
}
Vector3 Actor::GetRight() {
	float radians = GetRotation();
	float x = (float)(cos(radians + Math::PiOver2));//calculate the x component
	float y = (float)(sin(radians + Math::PiOver2));//calculate the y component, y is pointing down
	Vector3 v1(x, y, 0.0f);
	return v1;
}
Vector3 Actor::GetQuatForward() {
	Vector3 newVect = Vector3::Transform(Vector3::UnitX, myQuat);
	return Vector3::Normalize(newVect);
}
void Actor::CalcWorldTransform() {
	mWorldTransform = Matrix4::CreateScale(mScale) * Matrix4::CreateRotationZ(mRotation) * Matrix4::CreateFromQuaternion(myQuat) * Matrix4::CreateTranslation(mPosition);
	if (mParent != nullptr) {
		if (mInheritScale) {
			mWorldTransform *= mParent->mWorldTransform;
		}
		else {
			mWorldTransform *= mParent->GetWorldRotTrans();
		}
	}
}
Matrix4 Actor::GetWorldRotTrans() {
	if (mParent!=nullptr) {
		return Matrix4::CreateRotationZ(mRotation) * Matrix4::CreateFromQuaternion(myQuat) * Matrix4::CreateTranslation(mPosition) * mParent->GetWorldRotTrans();
	}
	else {
		return Matrix4::CreateRotationZ(mRotation) * Matrix4::CreateFromQuaternion(myQuat) * Matrix4::CreateTranslation(mPosition);
	}
}
Vector3 Actor::GetWorldPosition() {
	return mWorldTransform.GetTranslation();
}
Vector3 Actor::GetWorldForward() {
	return mWorldTransform.GetXAxis();
}
void Actor::AddChild(Actor* child) {
	mChildren.push_back(child);
}
void Actor::RemoveChild(Actor* child) {
	auto iter = std::find(mChildren.begin(), mChildren.end(), child);
	if (iter != mChildren.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mChildren.end() - 1;
		std::iter_swap(iter, iter2);
		mChildren.pop_back();
	}
}
