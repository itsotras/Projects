#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>
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

	class Player* GetPlayer() { return mPlayer; }
	void SetPlayer(Player* player) { mPlayer = player; }

	class std::vector<Actor*>& getBlocks() { return mBlocks; }
	class std::vector<class SecurityCamera*>& GetCameras() { return mCameras; }
	class std::queue<class Checkpoint*>& GetCheckpts() { return mCheckpts; }

	float GetTimer() { return timer; }
	int GetCoins() { return coins; }

	void SetTimer(float newTimer) { timer = newTimer; }
	void SetCoins(int newCoins) { coins = newCoins; }

	void SetNextLevelString(std::string level) { mNextLevel = level; }
	void LoadNextLevel();

	float GetTextTimer() { return textTimer; }
	void SetTextTimer(float newTime) { textTimer = newTime; }

	std::string GetCheckptText() { return checkptText; }
	void SetCheckptText(std::string text) { checkptText = text; }

private:
	class Player* mPlayer;

	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, Mix_Chunk*> mSounds;

	int coins = 0;
	float timer = 0;

	// All the actors in the game
	std::vector<class Actor*> mActors;
	//All the blocks in the game
	std::vector<class Actor*> mBlocks;

	class Renderer* mRenderer = nullptr;

	std::vector<class SecurityCamera*> mCameras;

	Uint32 mTicksCount = 0;
	bool mIsRunning;
	std::queue<class Checkpoint*> mCheckpts;

	std::string mNextLevel;

	class Arrow* myArrow;

	std::string checkptText = "";

	float textTimer = 0.0f;
};
