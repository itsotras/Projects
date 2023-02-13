#pragma once
#include "VehicleMove.h"
#include "Math.h"
#include <vector>
class EnemyMove : public VehicleMove {
public:
	EnemyMove(Actor* owner);
	void Update(float deltaTime) override;
private:
	std::vector<Vector3> myPoints;
	int nextPoint = 1;
};