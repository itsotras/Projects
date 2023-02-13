#include "VehicleMove.h"
#include "Actor.h"
#include "Game.h"
#include "HeightMap.h"
#include <fstream>
#include <sstream>
#include "CSVHelper.h"

VehicleMove::VehicleMove(Actor* owner) : Component(owner, 50) {
	std::ifstream csvRead;
	csvRead.open("Assets/HeightMap/Checkpoints.csv");
	std::string line;
	while (getline(csvRead, line)) {
		if (line != "") {
			std::vector<std::string> rowOfInts = CSVHelper::Split(line);
			if (rowOfInts[0] == "Type") {
				continue;
			}
			Checkpoint myCheckpoint;
			myCheckpoint.minX = std::stoi(rowOfInts[1]);
			myCheckpoint.maxX = std::stoi(rowOfInts[2]);
			myCheckpoint.minY = std::stoi(rowOfInts[3]);
			myCheckpoint.maxY = std::stoi(rowOfInts[4]);
			checkpoints.push_back(myCheckpoint);
		}
	}
}

void VehicleMove::Update(float deltaTime) {
	//linear Velocity
	if (accelPressed) {
		if (accelerateTime > accelRampTime) {
			accelerateTime = accelRampTime;
		}
		float accelerationMagnitude = Math::Lerp(minAccelMagnitude, maxAccelMagnitude, accelerateTime/accelRampTime);
		accelerationMagnitude = Math::Clamp(accelerationMagnitude, minAccelMagnitude, maxAccelMagnitude);
		mVelocity += mOwner->GetForward(mOwner->GetRotation()) * accelerationMagnitude * deltaTime;
		accelerateTime += deltaTime;
	}
	else {
		accelerateTime = 0.0f;
	}

	Vector3 changedPos= (mOwner->GetPosition() + mVelocity * deltaTime);
	if (mOwner->GetGame()->GetHeightMap()->IsOnTrack(Vector2(mOwner->GetPosition().x, mOwner->GetPosition().y))) {
		float newZ = Math::Lerp(mOwner->GetPosition().z, mOwner->GetGame()->GetHeightMap()->GetHeight(Vector2(mOwner->GetPosition().x, mOwner->GetPosition().y)), .1f);
		Vector3 newPos = changedPos;
		newPos.z = newZ;
		changedPos = newPos;
	}
	mOwner->SetPosition(changedPos);

	if (accelPressed) {
		mVelocity *= linearDrag;
	}
	else {
		mVelocity *= linearDragNoAccel;
	}

	//angular Velocity
	
	angularVelocity += angularAccel * mTurnState * deltaTime;
	mOwner->SetRotation(mOwner->GetRotation() + angularVelocity * deltaTime);
	angularVelocity *= angularDrag;

	Checkpoint nextCheckpoint;
	if (lastCheckpoint + 1 == (int)(checkpoints.size())) {
		nextCheckpoint = checkpoints[0];
	}
	else {
		nextCheckpoint = checkpoints[lastCheckpoint + 1];
	}
	if (hitCheckpoint(nextCheckpoint)) {
		lastCheckpoint++;
		if (lastCheckpoint == (int)(checkpoints.size())) {
			lastCheckpoint = 0;
		}
		if (lastCheckpoint == 0) {
			currentLap++;
			OnLapChange(currentLap);
		}
	}
}

bool VehicleMove::hitCheckpoint(Checkpoint newCheckPoint) {
	Vector3 position = mOwner->GetPosition();
	Vector2 position2 = Vector2(position.x, position.y);
	Vector2 myPos = mOwner->GetGame()->GetHeightMap()->WorldToCell(position2);
	for (int x = newCheckPoint.minX; x<= newCheckPoint.maxX; x++) {
		for (int y = newCheckPoint.minY; y <= newCheckPoint.maxY; y++) {

			if ((int)(myPos.x) == x) {
				if ((int)(myPos.y) == y) {
					return true;
				}
			}
		}
	}
	return false;
}

float VehicleMove::DistanceToNextCheckpoint() {
	int newCheckpoint = lastCheckpoint + 1;
	if (newCheckpoint == (int)(checkpoints.size())) {
		newCheckpoint = 0;
	}
	int row = checkpoints[newCheckpoint].minX + checkpoints[newCheckpoint].maxX;
	int col = checkpoints[newCheckpoint].minY + checkpoints[newCheckpoint].maxY;
	row /= 2;
	col /= 2;
	Vector3 checkpointPos = mOwner->GetGame()->GetHeightMap()->CellToWorld(row, col);
	return Vector3::Distance(mOwner->GetPosition(), checkpointPos);
}