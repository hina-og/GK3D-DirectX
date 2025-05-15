#pragma once
#include <vector>
#include "Engine/GameObject.h"

const int WIDTH{ 7 }, HEIGHT{ 11 };

struct MAPCHIP
{
	int tileModel_;
	int value_;
	XMFLOAT3 pos_;
	bool select_;
	bool onPlayer_;
};

class Stage
	: public GameObject
{
	MAPCHIP mapData_[HEIGHT][WIDTH];
	std::vector<std::vector<MAPCHIP>> map_;
public:
	Stage(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	bool HasPlayer(XMFLOAT2 _num);
	XMFLOAT3 SelectTilePosition(XMFLOAT2 _pos);
	XMFLOAT2 SelectTileNumber(XMFLOAT2 _pos);
};

