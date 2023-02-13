#include "HUD.h"
#include "Shader.h"
#include "Font.h"
#include "Texture.h"
#include <sstream>
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "SecurityCamera.h"

HUD::HUD(Actor* owner):UIComponent(owner) {
	mFont = new Font();
	mFont->Load("Assets/Inconsolata-Regular.ttf");
	mTimerText = mFont->RenderText("00:00.00");
	std::ostringstream oss;
	oss << "0"<<mOwner->GetGame()->GetCoins() << "/55";
	mCoinText = mFont->RenderText(oss.str());
	mCheckText = nullptr;
	UpdateCheckText();
	mRadarBg = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/Radar.png");
	mRadarArrow = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/RadarArrow.png");
	mBlip = mOwner->GetGame()->GetRenderer()->GetTexture("Assets/Blip.png");
}
HUD::~HUD() {
	mFont->Unload();
	delete mFont;
}
void HUD::Update(float deltaTime) {
	if (mOwner->GetGame()->GetTextTimer() <= 5.0f) {
		mOwner->GetGame()->SetTextTimer(mOwner->GetGame()->GetTextTimer() + deltaTime);
	}
	mOwner->GetGame()->SetTimer(mOwner->GetGame()->GetTimer() + deltaTime);
	float timer = mOwner->GetGame()->GetTimer();
	mTimerText->Unload();
	delete mTimerText;
	std::ostringstream oss;
	std::string myString;
	int fseconds = 0;
	int seconds = 0;
	int minutes = 0;
	if (timer <1.0f) {
		fseconds = (int)(timer * 100);
	}
	else if (timer > 1.0f && timer < 60.0f) {
		seconds = int(timer);
		fseconds = (int)((float)(timer - seconds) * 100);
	}
	if (timer > 60.0f) {//minutes moment
		int mins = (int)timer;
		minutes = mins / 60;
		seconds = int(timer - minutes*60);
		fseconds = (int)((float)(timer - minutes * 60 -seconds) * 100);
	}
	else {
		minutes = 0;
	}
	std::string fsecondBuffer = "";
	std::string secondBuffer = "";
	std::string minuteBuffer = "";
	if (fseconds < 10) {
		fsecondBuffer = "0";
	}
	if (seconds < 10) {
		secondBuffer = "0";
	}
	if (minutes < 10) {
		minuteBuffer = "0";
	}
	oss << minuteBuffer <<minutes<<":" << secondBuffer << seconds <<"."<<fsecondBuffer<< fseconds;
	myString = oss.str();
	mTimerText = mFont->RenderText(myString);
}

void HUD::Draw(Shader* shader) {
	DrawTexture(shader, mTimerText, Vector2(-420.0f, -325.0f));
	DrawTexture(shader, mCoinText, Vector2(-430.0f, -300.0f));
	if (mOwner->GetGame()->GetTextTimer() <= 5.0f && mCheckText!=nullptr) {
		DrawTexture(shader, mCheckText, Vector2::Zero);
	}
	DrawTexture(shader, mRadarBg, radarPos);
	DrawTexture(shader, mRadarArrow, radarPos);
	Vector3 playerFwd = mOwner->GetForward();
	float angle = Math::Atan2(playerFwd.y, playerFwd.x);
	Matrix3 rotation = Matrix3::CreateRotation(angle);
	Vector3 player3D = mOwner->GetPosition();
	Vector2 player2D = Vector2(player3D.y, player3D.x);
	for (auto camera : mOwner->GetGame()->GetCameras()) {
		Vector3 object3D = camera->GetPosition();
		Vector2 object2D = Vector2(object3D.y, object3D.x);
		Vector2 playerToObject = object2D - player2D;
		if (playerToObject.Length() <= radarRange) {
			playerToObject *= (radarRadius / radarRange);
			Vector2 newPos = Vector2::Transform(playerToObject, rotation);
			Vector3 objFWD3D = camera->GetQuatForward();
			Vector2 objFWD2D = Vector2(objFWD3D.y, objFWD3D.x);
			Vector2 newVect = Vector2::Transform(objFWD2D, rotation);
			float angle2 = Math::Atan2(newVect.y, newVect.x);
			DrawTexture(shader, mBlip, radarPos + newPos, 1.0f, angle2);
		}
	}
}

void HUD::GetCoin() {
	mCoinText->Unload();
	delete mCoinText;
	std::ostringstream oss;
	mOwner->GetGame()->SetCoins(mOwner->GetGame()->GetCoins() + 1);
	if (mOwner->GetGame()->GetCoins() < 10) {
		oss << "0"<<mOwner->GetGame()->GetCoins() << "/55";
	}
	else {
		oss << mOwner->GetGame()->GetCoins() << "/55";
	}
	mCoinText = mFont->RenderText(oss.str());
}

void HUD::UpdateCheckText() {
	if (mCheckText != nullptr) {
		mCheckText->Unload();
		delete mCheckText;
	}
	mCheckText = mFont->RenderText(mOwner->GetGame()->GetCheckptText());
	mOwner->GetGame()->SetTextTimer(0.0f);
}