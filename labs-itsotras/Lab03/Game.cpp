//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include "Log.h"
#include "Frog.h"
#include "Vehicle.h"
#include <algorithm>
#include <SDL2/SDL_image.h>
#include "SpriteComponent.h"
#include <iostream>
#include <fstream>
#include "Random.h"
#include "WrappingMove.h"
#include "CollisionComponent.h"

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
	myWindow = SDL_CreateWindow("Frogger", 50, 50, windowW/2, windowH/2, 0);//create a window and assign it to myWindow member
	

	if (myWindow == nullptr) {//if the window is still NULL return false
		return false;
	}

	myRenderer = SDL_CreateRenderer(myWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);//create a renderer using mywindow and the accelerated and vsync flags


	if (myRenderer == nullptr) {//if the renderer is NULL, return false
		return false;
	}

	int check = SDL_RenderSetLogicalSize(myRenderer, windowW, windowH);

	if (check != 0) {
		return false;
	}

	IMG_Init(IMG_INIT_PNG);//loads the image
	
	LoadData();//loads in the data

	return true;

}

void Game::RunLoop() {//runs a loop of the game
	while (isRunning){

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
	for (auto sprite : mSprites) {
		
		if (sprite->IsVisible()) {
			sprite->Draw(myRenderer);
		}
	}

	SDL_SetRenderDrawColor(myRenderer, 255, 255, 255, 255);//set color to white

	SDL_RenderPresent(myRenderer);//present the renderer
	
}

void Game::AddActor(Actor* actor) {
	mActors.push_back(actor);//push_back the actor
}

void Game::RemoveActor(Actor* actor) {
	std::vector<Actor*>::iterator it = std::find(mActors.begin(), mActors.end(), actor);
	mActors.erase(it);//erase the actor
}


void Game::LoadData() {

	//set up the background
	Vector2 bg_pos;
	bg_pos.x = windowW/2.0f;
	bg_pos.y = windowH/2.0f;
	Actor* bg = new Actor(this);
	bg->SetPosition(bg_pos);
	SpriteComponent* star_sc = new SpriteComponent(bg, 10);

	star_sc->SetTexture(GetTexture("Assets/Background.png"));

	std::ifstream ifS;
	ifS.open("Assets/Level.txt");
	std::string myLine;
	float startX = 64.0f;
	float startY = 160.0f;
	int lineNum = 0;
	while (getline(ifS, myLine)) {
		
		for (int i = 0; i < (int)(myLine.size()); i++) {
			startX = 64.0f + i * 64.0f;
			char myChar = myLine[i];
			
			if (myChar == '.') {
				//do nothing
			}
			else if (myChar == 'G') {
				//do nothing
				Vector2 goal_pos;
				goal_pos.x = startX;
				goal_pos.y = startY;
				mGoal = new Actor(this);
				mGoal->SetPosition(goal_pos);

				CollisionComponent* myCollider = new CollisionComponent(mGoal);
				myCollider->SetSize(64.0f, 64.0f);
			}
			else {
				if (myChar == 'X' || myChar == 'Y'|| myChar == 'Z') {
					Vector2 log_pos;
					log_pos.x = startX;
					log_pos.y = startY;
					Log* myLog = new Log(this, myChar, lineNum);
					myLog->SetPosition(log_pos);
				}
				else if (myChar == 'F') {
					Vector2 frog_pos;
					frog_pos.x = startX;
					frog_pos.y = startY;
					Frog* myFrog = new Frog(this);
					myFrog->SetPosition(frog_pos);
					myFrog->startPos = frog_pos;
					gameFrog = myFrog;
				}
				else {
					Vector2 car_pos;
					car_pos.x = startX;
					car_pos.y = startY;
					Vehicle* myCar = new Vehicle(this, myChar, lineNum);
					myCar->SetPosition(car_pos);
				}
			}
		}
		startY += 64.0f;
		lineNum += 1;//increase the row we are at
	}

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

		SDL_FreeSurface(surface);

		myMap.insert(std::make_pair(name, myTexture));

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

float Game::getWindowHeight() {
	return (float)(windowH);
}

float Game::getWindowWidth() {
	return (float)(windowW);
}

void Game::AddVehicle(Vehicle* newVehicle) {
	mVehicles.push_back(newVehicle);//push back the vehicle pointer
}

void Game::RemoveVehicle(Vehicle* newVehicle) {
	std::vector<Vehicle*>::iterator it = std::find(mVehicles.begin(), mVehicles.end(), newVehicle);
	mVehicles.erase(it);//erase the vehicle
}

Frog* Game::getFrog() {
	return gameFrog;
}

void Game::AddLog(Log* newLog) {
	mLogs.push_back(newLog);//push back the vehicle pointer
}

void Game::RemoveLog(Log* newLog) {
	std::vector<Log*>::iterator it = std::find(mLogs.begin(), mLogs.end(), newLog);
	mLogs.erase(it);//erase the vehicle
}

Actor* Game::getGoal() {
	return mGoal;
}
