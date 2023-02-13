#include "PlayerMove.h"
#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "HeightMap.h"
#include <SDL2/SDL_scancode.h>
#include "SDL2/SDL_mixer.h"
#include "PlayerUI.h"
#include "Enemy.h"

PlayerMove::PlayerMove(Actor* owner) : VehicleMove(owner) {
	mOwner->SetPosition(mOwner->GetGame()->GetHeightMap()->CellToWorld(39,58));
}

void PlayerMove::Update(float deltaTime) {

	//change the position of player
	VehicleMove::Update(deltaTime);
}

void PlayerMove::ProcessInput(const Uint8* keyState) {
	if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP]) {
		setPedal(true);
	}
	else {
		setPedal(false);
	}

	if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]) {
		setTurnState(VehicleState::Left);
	}
	else if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]) {
		setTurnState(VehicleState::Right);
	}
	else {
		setTurnState(VehicleState::None);
	}
	if ((keyState[SDL_SCANCODE_A] && keyState[SDL_SCANCODE_D]) || (keyState[SDL_SCANCODE_LEFT] && keyState[SDL_SCANCODE_RIGHT])) {
		setTurnState(VehicleState::None);
	}
	
}

void PlayerMove::OnLapChange(int newLap) { 
	if (newLap == 5) {
		Mix_FadeOutChannel(mOwner->GetGame()->GetMusicChannel(), 250);
		if (newLap > mOwner->GetGame()->GetEnemy()->GetComponent<VehicleMove>()->currentLap) {
			mOwner->GetGame()->GetPlayer()->getUI()->SetRaceState(PlayerUI::Won);
			Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Won.wav"), 0);
			
		}
		else {
			mOwner->GetGame()->GetPlayer()->getUI()->SetRaceState(PlayerUI::Lost);
			Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Lost.wav"), 0);
		}
		mOwner->SetState(ActorState::Paused);
		mOwner->GetGame()->GetEnemy()->SetState(ActorState::Paused);
	}
	else {
		if (newLap == 4){
			Mix_FadeOutChannel(mOwner->GetGame()->GetMusicChannel(), 250);
			Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/FinalLap.wav"), 0);
			mOwner->GetGame()->SetMusicChannel(Mix_FadeInChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/MusicFast.ogg"), -1, 4000));
			
		}
		mOwner->GetGame()->GetPlayer()->getUI()->OnLapChange(newLap);
	}
	
}
