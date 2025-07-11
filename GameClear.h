#pragma once
#include "Engine/GameObject.h"

class GameClear
	: public GameObject
{
	int hBord_;
public:
	GameClear(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();
};

