#pragma once
#include "SDL2/SDL.h"
#include <vector>
#include <unordered_map>
#include <string>

class Game {
public:
	Game(); //Game Constructor

	bool Initialize(); //Initializes the game

	void Shutdown(); //Shuts down the game

	void RunLoop(); //Runs the game loop

	void AddActor(class Actor* actor);//add actor to game

	void RemoveActor(class Actor* actor);//remove actor from game

	void AddAsteroid(class Asteroid* asteroid);

	void RemoveAsteroid(class Asteroid* asteroid);

	SDL_Texture* GetTexture(std::string name);//searches and return texture, unless its not in the map, where it creates and adds it to the map

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	std::vector<class Asteroid*> mAsteroids;//vector of Asteroids


	int window_h = 768; //height of the window
	int window_w = 1024; //width of the window

private:

	void LoadData();//load data
	void UnloadData();//unload data


	std::vector<class Actor*> mActors;

	SDL_Window* myWindow;//my window member
	SDL_Renderer* myRenderer;//my renderer member

	float prevTime = 0.0f;



	bool isRunning;

	std::unordered_map<std::string, SDL_Texture*> myMap;//map of textures

	std::vector<class SpriteComponent*> mSprites;//vector of spriteComponents
	

	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

};