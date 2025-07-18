#pragma once
#include "Engine/GameObject.h"
#include "Mouse.h"
#include "Zombie.h"
#include "Unit.h"
#include <vector>

class Player
	: public GameObject
{
	//�ǂ̑ϋv�l
	int HP;
public:
	Unit* unit_;

	Player(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

};

