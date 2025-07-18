#pragma once
#include "Engine/GameObject.h"
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include "MaterialTable.h"
#include "HUD.h"

//敵が出てくる時間
const float ENEMY_SPAWN_INTERVAL{ 10.0f };

class TutorialScene :
    public GameObject
{
	//BGMハンドル
	int hBGM_;
	//時間が動くか
	bool isTimeStart;
	//キャラの向き
	int selectDir;

	/*ポインタ*/
	Stage* stage;//ステージ
	Player* player;//プレイヤーの操作関係
	Enemy* enemy;//敵関係
	MaterialTable* material;//素材関係
	HUD* hud;//体力とか時間とかの表示

	//選択しているタイル
	XMFLOAT3 selectPos_;

	bool isReady;
	bool isBattleEnd;
public:
    TutorialScene(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};
