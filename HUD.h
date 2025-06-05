#pragma once
#include "Engine/GameObject.h"

class HUD
	: public GameObject
{
	int maxHP;//‘Ï‹v‚ÌÅ‘å’l

	int hHitPoint_;
	int hBaceHitPoint_;

public:
	int HP_;

	HUD(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	void InitHP(int _hp);
};

