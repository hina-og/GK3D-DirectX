#pragma once
#include <string>
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class HUD
	: public GameObject
{
	int maxHP;//ëœãvÇÃç≈ëÂíl

	int hHitPoint_;
	int hBaseHitPoint_;
	Transform transformHP_;
	

	int hTimeBase_;
	Text* timeText;
	Transform transformTime_;
	Transform transformTimeText_;
public:
	int HP_;
	float time_;

	HUD(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	void InitHP(int _hp);
	void TimeUpdate();
	std::string ToMinutesString();
};

