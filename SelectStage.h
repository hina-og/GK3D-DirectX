#pragma once
#include "Engine/GameObject.h"
#include "Calendar.h"

class SelectStage
	: public GameObject
{
	int stageNum_;
	int nowStage_;

	Calendar calendar;
public:
	bool decision_;

	SelectStage(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	void SetStageNum(int _num);
	int GetStageNum();
};

