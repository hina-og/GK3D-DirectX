#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include "MaterialTable.h"
#include "HUD.h"

class PlayScene
	: public GameObject
{
	//素材がもらえる秒数（現在    3秒  に1回もらえる）
	const float GiveMaterialTime{ 3.0f };

	enum StageDataLine
	{
		STAGE_DATA = 0,//ステージの基本情報の列
		SPAWN_DATA,//敵の出現情報の最初の列
	};

	//ステージの基本情報（csvファイルの1行目）
	enum StageData
	{
		TIME = 0,//時間
		HP,//体力
		GIVE_MATERIAL_NUM,//与えられる素材の数
	};

	//敵の出現情報（csvファイルの2行目以降）
	enum SpawnDataRow
	{
		SPAWN_NAME = 0,//敵の名前
		SPAWN_TIME,//出現する時間
		SPAWN_LINE,//出現する位置
	};

	//終了条件
	enum END_CONDITIONS
	{
		All_Kill,//敵がいない
		Time_Up,//最後まで守った
		Dead,//体力がなくなった
		END,//終了条件の個数
		//※追加する場合は絶対にENDを最後にする
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
	//BGMのハンドル
	int hBGM_;
	//時間が動いてもいいか（ロードが完了してから動かすため）
	bool isTimeStart;
	//素材入手のタイマー
	float getMaterialTime;
	//選択している方向
	int selectDir;

	//---各オブジェクトのポインタ---
	Stage* stage;
	Player* player;
	Enemy* enemy;
	MaterialTable* material;
	HUD* hud;
	//------------------------------


	//プレイヤーのキャラの配列
	std::vector<Puppet*> pPuppet_;
	//敵のキャラの配列
	std::vector<Puppet*> ePuppet_;

	//準備できたか
	bool isReady;
	//バトルが終わったか
	bool isBattleEnd;
public:
	PlayScene(GameObject* parent);
	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//描画
	void Draw() override;
	//開放
	void Release() override;
};

