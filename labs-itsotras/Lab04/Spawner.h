#include "Actor.h"

class Spawner : public Actor {
public:

	Spawner(Game* game);
private:
	void OnUpdate(float deltaTime);
	float distanceFromPlayer = 600.0f;
};