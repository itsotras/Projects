#pragma once
#include "Actor.h"
#include <string>

class Effect : public Actor {
public:
	Effect(class Game* game, Vector2 pos, std::string animName,std::string soundName);
	void OnUpdate(float deltaTime);
private:
	float lifeTime;
};