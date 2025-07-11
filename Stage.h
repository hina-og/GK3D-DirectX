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
	MAPCHIP beginData_;//左下（最初）のタイル
	MAPCHIP endData_;//右上（最後）のタイル

	//ハンドル
	int hGround_;//地面のモデル
	int hWall_;//壁のモデル
	int hTileSign_;//選択している場所をわかりやすくするやつ

	bool isSelect_;//どれか選択しているか

	bool isZooming_;//ズーム中か
	int zoomdist_;//ズームする距離

public:
	float spawnLine_;//敵が出てくる場所
	float wallLine_;//敵が壁にダメージを与える場所

	Stage(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	//引数_numのタイルを選択していて、プレイヤーキャラがいるか
	bool HasPlayer(XMFLOAT2 _num);
	//選択しているタイルの位置
	XMFLOAT3 SelectTilePosition(XMFLOAT2 _mousePos);
	//選択しているタイルのナンバー
	XMFLOAT2 SelectTileNumber(XMFLOAT2 _mousePos);

	/// <summary>
	/// タイルを選択しているかの判定をする
	/// </summary>
	/// <param name="_mousePos">マウスの位置</param>
	/// <param name="_outNum">選択している場合はそのタイルの番号を入れる</param>
	/// <param name="_outPos">選択している場合はそのタイルの位置を入れる</param>
	/// <returns>タイルを選択しているか</returns>
	bool SelectTile(XMFLOAT2 _mousePos, XMFLOAT2& _outNum, XMFLOAT3& _outPos);

	//プレイヤーキャラがいますよ状態にする
	void PlaceCharacter(XMFLOAT2 _num);

	/// <summary>
	/// 任意のタイルの位置
	/// </summary>
	/// <param name="_x">番号x</param>
	/// <param name="_y">番号y</param>
	/// <returns></returns>
	XMFLOAT3 TilePos(int _x, int _y);
	//ズームしているか
	bool IsZooming() { return isZooming_; }
};

