#include "EnemyMove.h"
#include "CSVHelper.h"
#include "fstream"
#include "sstream"
#include "Actor.h"
#include "Game.h"
#include "HeightMap.h"

EnemyMove::EnemyMove(Actor* owner) :VehicleMove(owner) {
	std::ifstream csvRead;
	csvRead.open("Assets/HeightMap/Path.csv");
	std::string line;
	while (getline(csvRead, line)) {
		if (line != "") {
			std::vector<std::string> rowOfInts = CSVHelper::Split(line);
			if (rowOfInts[0] == "Type") {
				continue;
			}
			int x = std::stoi(rowOfInts[1]);
			int y = std::stoi(rowOfInts[2]);
			Vector3 mPos = mOwner->GetGame()->GetHeightMap()->CellToWorld(x, y);
			myPoints.push_back(mPos);
		}
	}
	mOwner->SetPosition(myPoints[0]);
}
void EnemyMove::Update(float deltaTime) {
	if (Vector3::Distance(mOwner->GetPosition(), myPoints[nextPoint]) <=50.0f ){
		nextPoint++;
		if (nextPoint == (int)(myPoints.size())) {
			nextPoint = 0;
		}
	}
	Vector3 normalized = Vector3::Normalize(myPoints[nextPoint] - mOwner->GetPosition());
	float dot = Vector3::Dot(normalized, mOwner->GetForward(mOwner->GetRotation()));
	if ( dot <= 1.15f && dot >= 0.85f) {
		setPedal(true);
	}
	else {
		setPedal(false);
	}

	if (dot > 0.0f) {
		Vector3 cross = Vector3::Cross(mOwner->GetForward(mOwner->GetRotation()), normalized);
		if (cross.z > 0) {
			setTurnState(VehicleState::Right);
		}
		else {
			setTurnState(VehicleState::Left);
		}
	}
	else {
		setTurnState(VehicleState::None);
	}



	VehicleMove::Update(deltaTime);
}