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
	void AddBlock(class Block* block);
	void RemoveBlock(class Block* block);
	void AddEnemy(class Goomba* enemy);
	void RemoveEnemy(class Goomba* enemy);
	Actor* getGoal();
	class Player* getPlayer();
	float getWindowWidth();
	float getWindowHeight();
	const std::vector<Block*>& GetBlocks();
	const std::vector<Goomba*>& GetEnemies();
	Vector2& GetCameraPos();
	void killPlayer();
	Mix_Chunk* GetSound(const std::string& filename);
	int getChannel();

private:
	void LoadData();//load data
	void UnloadData();//unload data
	std::vector<class Actor*> mActors;
	Vector2 camera_pos = Vector2::Zero;
	SDL_Window* myWindow;//my window member
	SDL_Renderer* myRenderer;//my renderer member
	float prevTime = 0.0f;
	int windowH = 448; //height of the window
	int windowW = 600; //width of the window
	bool isRunning;
	class Player* mPlayer;
	int loopChannel;
	std::unordered_map<std::string, SDL_Texture*> myMap;//map of textures
	std::unordered_map<std::string, Mix_Chunk*> mySounds;//map of sounds
	std::vector<class SpriteComponent*> mSprites;//vector of spriteComponents
	std::vector<class Block*> mBlocks;//vector of blocks
	std::vector<class Goomba*> mEnemies;
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	float startingNum = 16.0f;
};