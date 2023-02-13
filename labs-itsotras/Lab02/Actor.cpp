#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
#include <cmath>

Actor::Actor(Game* game)
	:mGame(game)
	,mState(ActorState::Active)
	,mPosition(Vector2::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
{
	mGame->AddActor(this);//adds actor
	
}

Actor::~Actor()
{

	mGame->RemoveActor(this);//removes actor
	for (int i = 0; i <(int)(mComponents.size()); i++) {
		delete mComponents[i];//delete the pointers
	}
	mComponents.clear();//clear the vector of components
}

void Actor::Update(float deltaTime)
{

	if (mState == ActorState::Active) {
		for (int i = 0; i < (int)(mComponents.size()); i++) {
			mComponents[i]->Update(deltaTime);//update the component at index i
		}
		this->OnUpdate(deltaTime);
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

Vector2 Actor::GetForward(float radians) {
	float x = (float)(cos(radians));//calculate the x component
	float y = (float)( - 1 * sin(radians));//calculate the y component, y is pointing down
	Vector2 v1(x,y);
	return v1;
}
