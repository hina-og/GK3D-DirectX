#pragma once
#include <vector>
#include "Engine/GameObject.h"

const int WIDTH{ 7 }, HEIGHT{ 11 };
const float TILE_SIZE{ 1.0f };

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
	MAPCHIP beginData_, endData_;

	int hGround_;
	int hWall_;

	bool isZooming_;
	int zoomdist_;

public:
	float startLine_;
	float endLine_;

	Stage(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	bool HasPlayer(XMFLOAT2 _num);
	XMFLOAT3 SelectTilePosition(XMFLOAT2 _pos);
	XMFLOAT2 SelectTileNumber(XMFLOAT2 _pos);
	bool SelectTile(XMFLOAT2 _screenPos, XMFLOAT2& _outNum, XMFLOAT3& _outPos);
	void PlaceCharacter(XMFLOAT2 _num);
};

