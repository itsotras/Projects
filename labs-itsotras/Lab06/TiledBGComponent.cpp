#include "TiledBGComponent.h"
#include "Actor.h"
#include "Game.h"
#include <fstream>
#include "CSVHelper.h"
#include <iostream>
#include <sstream>

TiledBGComponent::TiledBGComponent(Actor* owner, int drawOrder): SpriteComponent(owner, drawOrder) {

}
TiledBGComponent::~TiledBGComponent() {

}
void TiledBGComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)
	{
		SDL_Rect r;
		r.w = tileW;
		r.h = tileH;
		SDL_Rect srcRect;
		srcRect.w = tileW;
		srcRect.h = tileH;
		int tilesPerRow = GetTexWidth() / tileW;
		//go through the csv file
		for (int x = 0; x <(int)(mCsvInts.size()); x++) {
			for (int y = 0; y < (int)(mCsvInts[x].size()); y++) {
				int myTextInt = mCsvInts[x][y];
				if (myTextInt != -1) {
					r.x = static_cast<int>(y*tileW - mOwner->GetGame()->GetCameraPos().x);
					r.y = static_cast<int>(x*tileH - mOwner->GetGame()->GetCameraPos().y);
					
					int row = myTextInt/tilesPerRow;
					int col = myTextInt%tilesPerRow;
					srcRect.x = col*tileW;
					srcRect.y = row*tileH;

					SDL_RenderCopyEx(renderer,
						mTexture,
						&srcRect,
						&r,
						0.0,
						nullptr,
						SDL_FLIP_NONE);
				}
			}
		}
	}
}
void TiledBGComponent::LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight) {
	tileW = tileWidth;
	tileH = tileHeight;
	std::ifstream csvRead;
	csvRead.open(fileName);
	std::string line;
	while (getline(csvRead, line)) {
		if (line != "") {
			std::vector<std::string> rowOfInts = CSVHelper::Split(line);
			std::vector<int> myInts;
			for (auto integer : rowOfInts) {
				//SDL_Log(integer.c_str());
				myInts.push_back(std::stoi(integer));
			}
			mCsvInts.push_back(myInts);
		}
	}
}