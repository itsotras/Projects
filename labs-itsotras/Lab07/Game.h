#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
#ifdef __EMSCRIPTEN__
	void EmRunIteration()
	{
		if (!mIsRunning)
		{
			emscripten_cancel_main_loop();
		}
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
#endif
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	Mix_Chunk* GetSound(const std::string& fileName);

	const float WINDOW_WIDTH = 1024.0f;
	const float WINDOW_HEIGHT = 768.0f;

	class Renderer* GetRenderer() {	return mRenderer; }

	std::vector<class Block*>& GetBlocks() { return mBlocks;  }

	class Player* GetPlayer() { return mPlayer; };

	void LoadBlocks(std::string filename, float x);

	int GetLoopChannel() { return shipChannel; }
private:
	class Player* mPlayer;

	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	// All the actors in the game
	std::vector<class Actor*> mActors;

	std::vector<class Block*> mBlocks;

	class Renderer* mRenderer = nullptr;

	int shipChannel;

	Uint32 mTicksCount = 0;
	bool mIsRunning;
};
