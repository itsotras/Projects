#include "Asteroid.h"
#include "Game.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Random.h"

Asteroid::Asteroid(Game* game)
	:Actor(game) {

	mMover = new MoveComponent(this);
	mMover->SetForwardSpeed(150.0f);
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(game->GetTexture("Assets/Asteroid.png"));
	mRotation = Random::GetFloatRange(0.0f, Math::TwoPi);
	Vector2 topLeft(0.0f, 0.0f);
	Vector2 botRight(float(mGame->window_w), float(mGame->window_w));
	mPosition = Random::GetVector(topLeft, botRight);

	game->AddAsteroid(this);
}

Asteroid::~Asteroid() {
	mGame->RemoveAsteroid(this);//removes actor
	for (int i = 0; i < (int)(mComponents.size()); i++) {
		delete mComponents[i];//delete the pointers
	}
	mComponents.clear();//clear the vector of components
}

void Asteroid::OnUpdate(float deltaTime) {
	if (mPosition.x > (float)(mGame->window_w)) {
		mPosition.x = 1.0f;
	}
	if (mPosition.x < 0.0f) {
		mPosition.x = (float)(mGame->window_w-1);
	}

	if (mPosition.y < 0.0f) {
		mPosition.y = (float)(mGame->window_h-1);
	}
	if (mPosition.y > (float)(mGame->window_h)) {
		mPosition.y = 1.0f;
	}
}