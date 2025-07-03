#pragma once
#include "Engine/GameObject.h"
#include "Calendar.h"

class SelectStage
	: public GameObject
{
	int stageNum_;//ステージの最大数
	int nowStage_;//現在選択しているステージ

	Calendar calendar;
public:
	bool decision_;//決定

	SelectStage(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();
	//ステージ数のセッター
	void SetStageNum(int _num);
	//ステージ数のゲッター
	int GetStageNum();
};

