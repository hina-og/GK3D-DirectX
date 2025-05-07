#pragma once
#include "Engine/GameObject.h"
#include "Mouse.h"
#include "Zombie.h"
#include "Unit.h"
#include <vector>

const float START_TIME{ 3.0 };

class Player
	: public GameObject
{
	

	XMFLOAT3 selectPos_;
	int HP;
public:
	Unit unit_;

	Player(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	void SetSelectTile(XMFLOAT3 _selectPos);
};

