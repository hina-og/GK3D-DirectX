#pragma once
#include "Puppet.h"
#include <vector>
#include "Engine/Image.h"

//表示するタイルの最大数
const int MAX_TILE_X{ 7 }, MAX_TILE_Y{ 7 };


class RangeView
{
	const XMINT2 RANGE_CENTER{ MAX_TILE_X / 2, MAX_TILE_Y / 2 };

	enum RangeType
	{
		ME = 0,
		DEFAULT,
		ATTACK,
		OTHER,
	};

	struct RangeTile
	{
		XMINT2 rangeData_;//攻撃範囲のデータ
		int type_;//タイルの種類
		int hRangeTile_;//タイルの画像ハンドル
	};

	RangeTile tile[MAX_TILE_Y][MAX_TILE_X];

	float imageSize;

public:
	void Initialize(XMFLOAT3 _position);
	void Draw();
	void SetData(std::vector<XMINT2> _data);
};

