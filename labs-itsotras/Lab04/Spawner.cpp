#include "Spawner.h"
#include "Game.h"
#include "Goomba.h"
#include "Player.h"


Spawner::Spawner(Game* game):Actor(game) {

}

void Spawner::OnUpdate(float deltaTime) {

	if (this->GetPosition().x - mGame->getPlayer()->GetPosition().x <= distanceFromPlayer) {
		Goomba* newGoom = new Goomba(mGame);
		newGoom->SetPosition(this->GetPosition());
		this->SetState(ActorState::Destroy);
	}

}