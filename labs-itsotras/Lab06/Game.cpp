//
//  Game.cpp
//  Game-mac
//
//  Created by Sanjay Madhav on 5/31/17.
//  Copyright © 2017 Sanjay Madhav. All rights reserved.
//

#include "Game.h"
#include "Actor.h"
#include "Player.h"
#include <algorithm>
#include <SDL2/SDL_image.h>
#include "SpriteComponent.h"
#include <iostream>
#include <fstream>
#include "Random.h"
#include "CollisionComponent.h"
#include "TiledBGComponent.h"
#include "CSVHelper.h"
#include "Collider.h"
#include "Bush.h"
#include "Soldier.h"
#include "PathFinder.h"
#include "PathNode.h"
#include "EnemyComponent.h"

Game::Game() {
	myWindow = nullptr;//initialize myWindow as nullptr
	myRenderer = nullptr;//initialize myRenderer as nullptr
	isRunning = true;//sets the isRunning to true
	startedSecond = false;
}
bool Game::Initialize() {
	Random::Init();
	int i = SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO); //initializes SDL with the audio and video subsystems
	if (i != 0) {//if this initialization doesn't work, return false
		return false;
	}
	myWindow = SDL_CreateWindow("The Legend of Zelda", 50, 50, windowW, windowH, 0);//create a window and assign it to myWindow member
	if (myWindow == nullptr) {//if the window is still NULL return false
		return false;
	}
	myRenderer = SDL_CreateRenderer(myWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);//create a renderer using mywindow and the accelerated and vsync flags
	if (myRenderer == nullptr) {//if the renderer is NULL, return false
		return false;
	}
	IMG_Init(IMG_INIT_PNG);//loads the image
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
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
	Mix_CloseAudio();
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
	if (!Mix_Playing(loopChannel) && !startedSecond) {
		Mix_Chunk* myChunk = GetSound("Assets/Sounds/MusicLoop.ogg");
		Mix_PlayChannel(-1, myChunk, -1);
		startedSecond = true;

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
	//SDL_SetRenderDrawColor(myRenderer, 255, 255, 255, 255);//set color to white
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
	Mix_Chunk* myChunk = GetSound("Assets/Sounds/MusicStart.ogg");
	loopChannel = Mix_PlayChannel(-1, myChunk, 0);
	mPathFinder = new PathFinder(this);
	Actor* myTiles = new Actor(this);
	TiledBGComponent* mTileComponent = new TiledBGComponent(myTiles);
	mTileComponent->LoadTileCSV("Assets/Map/Tiles.csv", 32, 32);
	mTileComponent->SetTexture(GetTexture("Assets/Map/Tiles.png"));
	LoadActors("Assets/Map/Objects.csv");
	
}
void Game::UnloadData() {
	while (!mActors.empty()) {
		delete mActors.back();
	}
	for (auto texture : myMap) {
		SDL_DestroyTexture(texture.second);
	}
	for (auto sound : mySounds) {
		Mix_FreeChunk(sound.second);
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
Vector2& Game::GetCameraPos() {
	return camera_pos;
}
Player* Game::getPlayer() {
	return mPlayer;
}
Mix_Chunk* Game::GetSound(const std::string& filename) {
	std::unordered_map<std::string, Mix_Chunk*>::iterator it = mySounds.find(filename);
	if (it != mySounds.end()) {//if the Mix Chunk is in the map
		return it->second;
	}
	else {
		Mix_Chunk* sound = Mix_LoadWAV(filename.c_str());
		if (sound == nullptr) {
			SDL_Log("%s ", filename.c_str());
		}
		mySounds.insert(std::make_pair(filename, sound));
		return sound;//make the sound, add it to the map and return the sound
	}
}
int Game::getChannel() {
	return loopChannel;
}
void Game::LoadActors(const std::string& fileName) {
	std::ifstream csvRead;
	csvRead.open(fileName);
	std::string line;
	getline(csvRead, line);
	while (getline(csvRead, line)) {
		if (line != "") {
			std::vector<std::string> attributes = CSVHelper::Split(line);
			Vector2 actorPos = Vector2((float)(std::stoi(attributes[1])+ std::stoi(attributes[3])/2), (float)(std::stoi(attributes[2]) + std::stoi(attributes[4]) / 2));
			if (attributes[0] == "Player") {//load in the player
				mPlayer = new Player(this);
				mPlayer->SetPosition(actorPos);
			}
			else if (attributes[0] == "Collider") {//load in the colliders
				Collider* newCollider = new Collider(this, (float)(std::stoi(attributes[3])), (float)(std::stoi(attributes[4])));
				newCollider->SetPosition(actorPos);
				mColliders.push_back(newCollider);
			}
			else if (attributes[0] == "Bush") {//load in the bushes
				Bush* newBush = new Bush(this);
				newBush->SetPosition(actorPos);
			}
			else if (attributes[0] == "Soldier") {//load in the soldiers
				PathNode* startNode = mPathFinder->GetPathNode(std::stoi(attributes[5]), std::stoi(attributes[6]));
				PathNode* endNode = mPathFinder->GetPathNode(std::stoi(attributes[7]), std::stoi(attributes[8]));
				Soldier* newSoldier = new Soldier(this, startNode, endNode);
				newSoldier->SetPosition(actorPos);
			}
		}
	}
}
const std::vector<Collider*>& Game::getColliders() {
	return mColliders;
}
PathFinder* Game::getPathFinder() {
	return mPathFinder;
}

std::vector<EnemyComponent*>& Game::getEnemyComps() {
	return mEnemyComps;
}