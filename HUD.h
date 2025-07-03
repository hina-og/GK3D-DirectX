#pragma once
#include <string>
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class HUD
	: public GameObject
{
	//csvÇÃè„Ç©ÇÁèáî‘
	enum IMAGE_ROW
	{
		BASE_HP = 1,
		HP,
		BASE_TIME,
		TIME,
		DIRECTION,
	};

	int maxHP;//ëœãvÇÃç≈ëÂíl

	int hHitPoint_;
	int hBaseHitPoint_;
	Transform transformHP_;
	
	int hTimeBase_;
	Text* timeText;
	Transform transformTime_;
	Transform transformTimeText_;

	int hModelDirection_;
	Transform transformArrow_;
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
	void SetDirection(int _dir);
};

