#pragma once
#include <vector>

class HeightMap {
public:
	HeightMap();
	class Vector3 CellToWorld(int row, int col);
	class Vector2 WorldToCell(const class Vector2& pos);
	bool IsOnTrack(const class Vector2& pos);
	float GetHeight(const class Vector2& pos);
private:
	bool IsCellOnTrack(int row, int col);
	float GetHeightFromCell(int row, int col);
	
	const float CELL_SIZE = 40.55f;
	const float GRID_TOP = 1280.0f;
	const float GRID_LEFT = -1641.0f;

	std::vector<std::vector<int>> myHeights;
 };