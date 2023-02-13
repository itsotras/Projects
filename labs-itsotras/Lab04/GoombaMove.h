#include "MoveComponent.h"


class GoombaMove : public MoveComponent {
public:
	GoombaMove(Actor* owner);
	~GoombaMove();
	void Update(float deltaTime);
private:
	float direction = -1.0f;
	float mYSpeed = 0.0f;
	float limit = 448.0f;
	float gravity = 2000.0f;
};