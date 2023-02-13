#include "HeightMap.h"
#include <fstream>
#include <iostream>
#include "CSVHelper.h"
#include "Math.h"
#include "SDL2/SDL.h" 

HeightMap::HeightMap() {
	std::ifstream csvRead;
	csvRead.open("Assets/HeightMap/HeightMap.csv");
	std::string line;
	while (getline(csvRead, line)) {
		if (line != "") {
			std::vector<std::string> rowOfInts = CSVHelper::Split(line);
			std::vector<int> myInts;
			for (auto integer : rowOfInts) {
				myInts.push_back(std::stoi(integer));
			}
			myHeights.push_back(myInts);
		}
	}

}

bool HeightMap::IsCellOnTrack(int row, int col) {
	if (row >=(int) (myHeights.size()) || row < 0) {
		return false;
	}
	if (col >= (int)(myHeights[0].size()) || col < 0) {
		return false;
	}
	int val = myHeights[row][col];
	if (val == -1) {
		return false;
	}
	return true;
}

float HeightMap::GetHeightFromCell(int row, int col) {
	int cellValue = myHeights[row][col];
	return -40.0f + cellValue * 5.0f;
}

Vector3 HeightMap::CellToWorld(int row, int col) {
	if (!IsCellOnTrack(row, col)) {
		return Vector3::Zero;
	}
	float x = GRID_TOP - CELL_SIZE * row;
	float y = GRID_LEFT + CELL_SIZE * col;
	float z = GetHeightFromCell(row, col);

	return Vector3(x, y, z);
}

Vector2 HeightMap::WorldToCell(const Vector2& pos) {

	float value1 = pos.x - GRID_TOP;
	float value2 = pos.y - GRID_LEFT;
	
	float row = (Math::Abs(value1) + CELL_SIZE / 2.0f) / CELL_SIZE;
	float col = (Math::Abs(value2) + CELL_SIZE / 2.0f) / CELL_SIZE;

	return Vector2(row, col);
}

bool HeightMap::IsOnTrack(const Vector2& pos) {
	Vector2 newPos = WorldToCell(pos);
	return IsCellOnTrack((int)(newPos.x), (int)(newPos.y));
}

float HeightMap::GetHeight(const Vector2& pos) {
	if (IsOnTrack(pos)) {
		Vector2 newPos = WorldToCell(pos);
		return GetHeightFromCell((int)(newPos.x), (int)(newPos.y));
	}
	else {
		float num = -1000.0f;
		return num;
	}
}