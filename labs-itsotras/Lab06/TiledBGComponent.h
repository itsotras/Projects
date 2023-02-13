#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <string>

class TiledBGComponent : public SpriteComponent {
public:
	TiledBGComponent(class Actor* owner, int drawOwner = 50);
	~TiledBGComponent();
	void Draw(SDL_Renderer* renderer);
	void LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight);

private:
	int tileW = 0;
	int tileH = 0;
	std::vector<std::vector<int>> mCsvInts;
};