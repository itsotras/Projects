#pragma once
#include "SDL2/SDL.h"
#include <vector>
#include <unordered_map>
#include <string>

// TODO
class Game {
public:
	Game(); //Game Constructor

	bool Initialize(); //Initializes the game

	void Shutdown(); //Shuts down the game

	void RunLoop(); //Runs the game loop

	void AddActor(class Actor* actor);//add actor to game

	void RemoveActor(class Actor* actor);//remove actor from game

	SDL_Texture* GetTexture(std::string name);//searches and return texture, unless its not in the map, where it creates and adds it to the map

	void AddSprite(class SpriteComponent* sprite);

	void RemoveSprite(class SpriteComponent* sprite);

	void AddVehicle(class Vehicle* newVehicle);

	void RemoveVehicle(class Vehicle* newVehicle);

	void AddLog(class Log* newLog);

	void RemoveLog(class Log* newLog);

	std::vector<Vehicle*>& getMyVehicles() { return mVehicles; };

	std::vector<Log*>& getMyLogs() { return mLogs;  };

	Actor* getGoal();

	float getWindowWidth();

	float getWindowHeight();

	class Frog* getFrog();

private:
	void LoadData();//load data

	void UnloadData();//unload data

	std::vector<class Actor*> mActors;

	std::vector<class Vehicle*> mVehicles;//vector of vehicles

	std::vector<class Log*> mLogs;// vector of Logs

	Actor* mGoal;

	SDL_Window* myWindow;//my window member
	SDL_Renderer* myRenderer;//my renderer member

	float prevTime = 0.0f;

	int windowH = 1024; //height of the window
	int windowW = 896; //width of the window

	bool isRunning;

	class Frog* gameFrog;

	std::unordered_map<std::string, SDL_Texture*> myMap;//map of textures

	std::vector<class SpriteComponent*> mSprites;//vector of spriteComponents
	
	void ProcessInput();

	void UpdateGame();

	void GenerateOutput();

};