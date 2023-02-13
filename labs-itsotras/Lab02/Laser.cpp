#include "Laser.h"
#include "Game.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Asteroid.h"

Laser::Laser(Game* game)
	:Actor(game) {

	mMover = new MoveComponent(this);
	mMover->SetForwardSpeed(400.0f);
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(game->GetTexture("Assets/Laser.png"));
	
}

void Laser::OnUpdate(float deltaTime) {
	if (lifeTime > 0.0f) {
		lifeTime -= deltaTime;
	}
	if (lifeTime <= 0.0f) {
		mState = ActorState::Destroy;
	}

	Vector2 mPos = GetPosition();
	
	std::vector<Asteroid*>& refAsteroids = mGame->mAsteroids;
	for (auto asteroid: refAsteroids) {
		Vector2 asPos = asteroid->GetPosition();

		if( Vector2::Distance(mPos, asPos) <= 70) {
			SetState(ActorState::Destroy);
			asteroid->SetState(ActorState::Destroy);
			break;
		}
	}

}