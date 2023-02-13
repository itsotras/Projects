#pragma once
#include "Component.h"
#include "Math.h"
#include <vector>

struct Checkpoint {
	int minX;
	int minY;
	int maxX;
	int maxY;
};

class VehicleMove : public Component {
public:
	enum VehicleState
	{
		Left = -1,
		Right = 1,
		None = 0
	};
	VehicleMove(class Actor* owner);
	void Update(float deltaTime) override;
	void setPedal(bool isPressed) { accelPressed = isPressed;  }
	void setTurnState(VehicleState newState) { mTurnState = newState;  }
	virtual void OnLapChange(int newLap) { }
	float DistanceToNextCheckpoint();
	int currentLap = 0;
	int lastCheckpoint = -1;
	bool hitCheckpoint(Checkpoint newCheckPoint);

private:
	bool accelPressed = false;
	VehicleState mTurnState = VehicleState::None;

	Vector3 mVelocity = Vector3::Zero;
	float angularVelocity = 0.0f;

	float accelerateTime = 0.0f;

	const float minAccelMagnitude = 1000.0f;
	const float maxAccelMagnitude = 2500.0f;
	const float accelRampTime = 2.0f;
	const float angularAccel = 5.0f * Math::Pi;
	const float linearDrag = 0.9f;
	const float linearDragNoAccel = 0.975f;
	float angularDrag = 0.9f;

	std::vector<Checkpoint> checkpoints;
};