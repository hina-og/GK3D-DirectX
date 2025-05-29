#pragma once
#include "Engine/GameObject.h"
#include "Calendar.h"

class SelectStage
	: public GameObject
{
	const int STAGE_NUM{ 3 };
	int nowStage_;

	Calendar calendar;
public:
	bool decision_;

	SelectStage(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	int GetStageNum();
};

