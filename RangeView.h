#pragma once
#include "Puppet.h"
#include <vector>
#include "Engine/Image.h"

//表示するタイルの最大数
const int MAX_TILE_X{ 7 }, MAX_TILE_Y{ 7 };

//攻撃範囲の表示
class RangeView
{
	//自分がいるマスの位置（中央）
	const XMINT2 RANGE_CENTER{ MAX_TILE_X / 2, MAX_TILE_Y / 2 };

	enum RangeType
	{
		ME = 0,//自分がいるマス
		DEFAULT,//何もないマス
		ATTACK,//攻撃するマス
		OTHER,//その他（今は使ってないけどデバフマスとか）
	};

	struct RangeTile
	{
		XMINT2 rangeData_;//タイルデータ
		int type_;//タイルの種類
		int hRangeTile_;//タイルの画像ハンドル
	};

	RangeTile tile[MAX_TILE_Y][MAX_TILE_X];

	//正方形のためfloat
	float imageSize_;

public:
	//初期化
	void Initialize(XMFLOAT3 _position);
	//描画
	void Draw();
	/// <summary>
	/// 攻撃範囲データをタイルにする
	/// </summary>
	/// <param name="_data">攻撃範囲のデータ</param>
	void SetData(std::vector<XMINT2> _data);
};

