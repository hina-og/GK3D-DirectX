#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include "Pot.h"
#include "MaterialTable.h"

class Battle
	: public GameObject
{
	Stage* stage;
	Player* player;
	Enemy* enemy;
	Pot* pot;
	MaterialTable* material;

	int mouseX;
	int mouseY;
	XMFLOAT3 selectPos_;

	bool prevLeftClick;

	std::vector<Puppet*> pPuppet_;
	std::vector<Puppet*> ePuppet_;
public:
	Battle(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	void SetSelectTile(XMFLOAT3 _selectPos);
	XMFLOAT3 GetSelectTile();
};

