#include "UIComponent.h"
#include <string>

class HUD : public UIComponent {
public:
	HUD(class Actor* owner);
	~HUD();
	void Update(float deltaTime);
	void Draw(class Shader* shader);
	void GetCoin();
	void UpdateCheckText();

	
private:
	class Font* mFont;
	class Texture* mTimerText;
	class Texture* mCoinText;
	class Texture* mCheckText;

	class Texture* mRadarBg;
	class Texture* mRadarArrow;
	class Texture* mBlip;
	Vector2 radarPos = Vector2(400.0f, -270.0f);
	float radarRange = 1500.0f;
	float radarRadius = 92.0f;//pixels

};