#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <vector>
#include <unordered_map>
#include "Math.h"
#include <string>


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
	Actor* getGoal();
	class Player* getPlayer();
	float getWindowWidth();
	float getWindowHeight();
	Vector2& GetCameraPos();
	Mix_Chunk* GetSound(const std::string& filename);
	int getChannel();
	void LoadActors(const std::string& filename);
	const std::vector<class Collider*>& getColliders();
	std::vector<class EnemyComponent*>& getEnemyComps();
	class PathFinder* getPathFinder();
private:
	void LoadData();//load data
	void UnloadData();//unload data
	std::vector<class Actor*> mActors;
	Vector2 camera_pos = Vector2::Zero;
	SDL_Window* myWindow;//my window member
	SDL_Renderer* myRenderer;//my renderer member
	float prevTime = 0.0f;
	int windowH = 448; //height of the window
	int windowW = 512; //width of the window
	bool isRunning;
	class Player* mPlayer;
	int loopChannel;
	std::unordered_map<std::string, SDL_Texture*> myMap;//map of textures
	std::unordered_map<std::string, Mix_Chunk*> mySounds;//map of sounds
	std::vector<class SpriteComponent*> mSprites;//vector of spriteComponents
	std::vector<class Collider*> mColliders;
	std::vector<class EnemyComponent*> mEnemyComps;
	class PathFinder* mPathFinder;
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	bool startedSecond = false;
};