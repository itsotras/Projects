//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include "Ship.h"
#include "Asteroid.h"
#include <algorithm>
#include <SDL2/SDL_image.h>
#include "SpriteComponent.h"
#include <iostream>
#include "Random.h"


Game::Game() {
	myWindow = nullptr;//initialize myWindow as nullptr
	myRenderer = nullptr;//initialize myRenderer as nullptr
	isRunning = true;//sets the isRunning to true
	
}

bool Game::Initialize() {

	Random::Init();

	int i = SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO); //initializes SDL with the audio and video subsystems

	if (i != 0) {//if this initialization doesn't work, return false
		return false;
	}
	myWindow = SDL_CreateWindow("Asteroids", 50, 50, window_w, window_h, 0);//create a window and assign it to myWindow member

	if (myWindow == nullptr) {//if the window is still NULL return false
		return false;
	}

	myRenderer = SDL_CreateRenderer(myWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);//create a renderer using mywindow and the accelerated and vsync flags


	if (myRenderer == nullptr) {//if the renderer is NULL, return false
		return false;
	}

	IMG_Init(IMG_INIT_PNG);//loads the image
	

	LoadData();//loads in the data

	return true;

}

void Game::RunLoop() {//runs a loop of the game
	while (isRunning) {
		ProcessInput();//processes input
		UpdateGame();//updates the game
		GenerateOutput();//generates the output
	}

}


void Game::Shutdown() {//shuts down the game
	UnloadData();//unloads the data in the game
	IMG_Quit();//destroys the image
	SDL_DestroyRenderer(myRenderer);//destroys the renderer
	SDL_DestroyWindow(myWindow);//destroys the window
	SDL_Quit();//quits SDL
}

void Game::ProcessInput() {
	SDL_Event new_event;

	while (SDL_PollEvent(&new_event)) {//poll for new events

		switch (new_event.type) {//switch for the event types

		case SDL_QUIT: //if you click the 'x', set isRunning to false
			isRunning = false;

		}
	}
	const Uint8* mystate = SDL_GetKeyboardState(NULL);//get the state of the keyboard

	std::vector<Actor*> copy = mActors;
	for (int i = 0; i<(int)(copy.size()); i++) {//loop through actors and process input
		copy[i]->ProcessInput(mystate);
	}

	if (mystate[SDL_SCANCODE_ESCAPE]) {//if the escape key is pressed, set isRunning to false
		isRunning = false;
	}


	
}
void Game::UpdateGame() {

	//calculate deltaTime

	int currTime = SDL_GetTicks();
	float deltaTime;
	while (currTime - prevTime < 16) {//loop until the difference of milliseconds is at least 16
		currTime = SDL_GetTicks();//try again
	}
	deltaTime = currTime - prevTime;//calculate deltaTime

	if (deltaTime > 33) {//set max before using deltaTime
		deltaTime = 33;
	}
	
	deltaTime /= 1000.0f; //convert to milliseconds
	prevTime = (float)currTime;//set prevTime to the currentTime


	std::vector<Actor*> copy_Actors = mActors;//make a copy of the actors vector
	for (int i = 0; i < (int)(copy_Actors.size()); i++) {
		copy_Actors[i]->Update(deltaTime);
	}
	std::vector<Actor*> temp;
	for (int i = 0; i < (int)(mActors.size()); i++) {
		if (mActors[i]->GetState() == ActorState::Destroy) {//if the actor is in destroy state
			temp.push_back(mActors[i]);//add to temp
		}
	}

	for (int i = 0; i <(int)(temp.size()); i++) {
		delete temp[i];//delete the destroyed actors
	}


	
	
}

void Game::GenerateOutput() {

	

	SDL_SetRenderDrawColor(myRenderer, 0, 0, 0, 255);//draw the background blue

	SDL_RenderClear(myRenderer);//clear the renderer

	//draw the game objects
	//SDL_Log("123123123");
	for (auto sprite : mSprites) {
		
		if (sprite->IsVisible()) {
			//std::cout << "Yo" << std::endl;
			sprite->Draw(myRenderer);
		}
	}

	


	SDL_RenderPresent(myRenderer);//present the renderer
	
}

void Game::AddActor(Actor* actor) {
	mActors.push_back(actor);//push_back the actor
}

void Game::RemoveActor(Actor* actor) {
	std::vector<Actor*>::iterator it = std::find(mActors.begin(), mActors.end(), actor);
	mActors.erase(it);//erase the actor
}

void Game::AddAsteroid(Asteroid* asteroid) {
	mAsteroids.push_back(asteroid);//push_back the Asteroid
}

void Game::RemoveAsteroid(Asteroid* asteroid) {
	std::vector<Asteroid*>::iterator it = std::find(mAsteroids.begin(), mAsteroids.end(), asteroid);
	mAsteroids.erase(it);//erase the Asteroid
}

void Game::LoadData() {


	//load in the ship
	Ship* ship = new Ship(this);
	Vector2 v(float(window_w/2), float(window_h/2));
	ship->SetPosition(v);

	for (int i = 0; i < 10; i++) {//load in the asteroids
		Asteroid* asteroid = new Asteroid(this);
		asteroid->GetState();
		
	}

	//load in the stars
	Vector2 star_pos;
	star_pos.x = float(window_w / 2);
	star_pos.y = float(window_h / 2);
	Actor* star = new Actor(this);
	star->SetPosition(star_pos);
	SpriteComponent* star_sc = new SpriteComponent(star, 10);
	
	star_sc->SetTexture(GetTexture("Assets/Stars.png"));

}
void Game::UnloadData() {
	while (mActors.empty() == false) {
		delete mActors.back();
	}

	for (auto texture : myMap) {
		SDL_DestroyTexture(texture.second);
	}
}

SDL_Texture* Game::GetTexture(std::string name) {
	std::unordered_map<std::string, SDL_Texture*>::iterator it =  myMap.find(name);
	
	if (it != myMap.end()){//if the SDL_TExture is in the map
		return it->second;
	}
	else {
		SDL_Surface* surface = IMG_Load(name.c_str());
		if (surface == nullptr) {
			SDL_Log("%s ", name.c_str());
		}
		SDL_Texture* myTexture = SDL_CreateTextureFromSurface(myRenderer, surface);
		//SDL_Log("444444");
		SDL_FreeSurface(surface);
		myMap.insert(std::make_pair(name, myTexture));
		//SDL_Log(name.c_str());
		return myTexture;//make the texture, add it to the map and return the texture
	}
}

void Game::AddSprite(SpriteComponent* sprite) {
	mSprites.push_back(sprite);//add a new sprite
	std::sort(mSprites.begin(), mSprites.end(),
		[](SpriteComponent* a, SpriteComponent* b) {
			return a->GetDrawOrder() < b->GetDrawOrder();
		});//sort the sprites
	
}

void Game::RemoveSprite(SpriteComponent* sprite) {
	std::vector<SpriteComponent*>::iterator it = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(it);//erase the sprite
}

