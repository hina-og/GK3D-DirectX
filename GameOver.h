#pragma once
#include "Engine/GameObject.h"

class GameOver
	: public GameObject
{
	int hBord_;
public:
	GameOver(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();
};

