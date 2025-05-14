#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include "Pot.h"
#include "MaterialTable.h"

class Battle
	: public GameObject
{
	struct SpawnData
	{
		int type_;//出てくるキャラの種類
		float time_;//出てくる残り時間
		int line_;//出てくる列
	};
	std::vector<SpawnData> spawnList_;
	int spawnedNum;

	const float INIT_BATTLE_TIME{ 120.0 };
	
	float time;
	Text timeText;
	bool isTimeStert;

	Stage* stage;
	Player* player;
	Enemy* enemy;
	Pot* pot;
	MaterialTable* material;

	int mouseX;
	int mouseY;
	XMFLOAT3 selectPos_;

	std::vector<Puppet*> pPuppet_;
	std::vector<Puppet*> ePuppet_;

	bool isReady;
public:
	Battle(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	void SetSelectTile(XMFLOAT3 _selectPos);
	XMFLOAT3 GetSelectTile();
};

