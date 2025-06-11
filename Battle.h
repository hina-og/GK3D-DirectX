#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include "MaterialTable.h"
#include "HUD.h"

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
	
	int hBGM_;

	bool isTimeStart;

	float getMaterialTime;

	int selectDir;

	Stage* stage;
	Player* player;
	Enemy* enemy;
	MaterialTable* material;
	HUD* hud;

	int mouseX;
	int mouseY;
	XMFLOAT3 selectPos_;

	std::vector<Puppet*> pPuppet_;
	std::vector<Puppet*> ePuppet_;

	bool isReady;
	bool isBattleEnd;
public:
	Battle(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	void SetSelectTile(XMFLOAT3 _selectPos);
	XMFLOAT3 GetSelectTile();
};

