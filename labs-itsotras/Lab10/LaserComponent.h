#pragma once
#include "MeshComponent.h"
#include "SegmentCast.h"

class LaserComponent :public MeshComponent {
public:
	LaserComponent(class Actor* owner);
	void Update(float deltaTime);
	void Draw(class Shader* shader);
	
private:
	Matrix4 GetMatrixFromSegment(LineSegment line);
	std::vector<LineSegment> mySegments;
};