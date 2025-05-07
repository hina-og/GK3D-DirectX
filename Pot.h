#pragma once
#include "Engine/GameObject.h"

class Pot
	: public GameObject
{
	int hPict_;
public:
	Pot(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

};

