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
	const float GiveMaterialTime{ 3.0f };

	enum StageDataLine
	{
		STAGE_DATA = 0,//ステージの基本情報の列
		SPAWN_DATA,
	};

	//ステージの基本情報
	enum StageData
	{
		TIME = 0,//時間
		HP,//体力
		GIVE_MATERIAL_NUM,//与えられる素材の数
	};

	enum SpawnDataRow
	{
		SPAWN_NAME = 0,
		SPAWN_TIME,
		SPAWN_LINE,
	};

	enum END_CONDITIONS
	{
		All_Kill,
		Time_Up,
		Dead,
		END,
	};

	struct SpawnData
	{
		int type_;//出てくるキャラの種類
		float time_;//出てくる残り時間
		int line_;//出てくる列
	};
	//敵出現リスト
	std::vector<SpawnData> spawnList_;
	//スポーンした敵の数
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

};

