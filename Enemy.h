#pragma once
#include "Engine/GameObject.h"
#include "Mouse.h"
#include "Zombie.h"
#include "Unit.h"
#include <vector>

class Enemy
	: public GameObject
{
public:
	Unit* unit_;

	Enemy(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

};

