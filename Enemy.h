#pragma once
#include "Engine/GameObject.h"
#include "Mouse.h"
#include "Zombie.h"
#include "Unit.h"
#include <vector>

const float ENEMY_START_TIME{ 3.0 };

class Enemy
	: public GameObject
{
	
	bool isAddEnemy_;

	bool prevLeftClick;
public:
	Unit unit_;

	Enemy(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

};

