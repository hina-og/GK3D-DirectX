#pragma once
#include <vector>
#include "Engine/GameObject.h"


//タイルの最大数
const int WIDTH{ 7 }, HEIGHT{ 11 };
//タイル１つの縦・横の幅（正方形だから同じ）
const float TILE_SIZE{ 1.0f };

struct MAPCHIP
{
	int hTile_;//モデルのハンドル
	int type_;//タイルの種類（今は使ってないけどいずれステージごとにギミックを追加できそう）
	XMFLOAT3 pos_;//タイルの位置
	bool select_;//このタイルを選択しているか
	bool onPlayer_;//プレイヤーキャラが乗っているか
};

class Stage
	: public GameObject
{
	MAPCHIP mapData_[HEIGHT][WIDTH];
	//最初のタイルと最後のタイルのデータ
	MAPCHIP beginData_, endData_;

	//ハンドル
	int hGround_;//地面のモデル
	int hWall_;//壁のモデル
	int hTileSign_;//選択している場所をわかりやすくするやつ

	bool isSelect_;//どれか選択しているか

	bool isZooming_;//ズーム中か
	int zoomdist_;//ズームする距離

public:
	float startLine_;//敵が出てくる場所
	float endLine_;//敵が壁にダメージを与える場所

	Stage(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	bool HasPlayer(XMFLOAT2 _num);
	XMFLOAT3 SelectTilePosition(XMFLOAT2 _pos);//選択しているタイルの位置
	XMFLOAT2 SelectTileNumber(XMFLOAT2 _pos);//選択しているタイルのナンバー
	bool SelectTile(XMFLOAT2 _screenPos, XMFLOAT2& _outNum, XMFLOAT3& _outPos);
	void PlaceCharacter(XMFLOAT2 _num);//プレイヤーキャラがいますよ状態にする
	bool IsZooming() { return isZooming_; }
};

