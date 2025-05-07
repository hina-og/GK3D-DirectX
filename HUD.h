#pragma once
#include "Engine/GameObject.h"
#include "PlayScene.h"
#include "Enemy.h"
#include <vector>
#include <string>
class HUD
	:public GameObject
{
	int hHUD_;
	int hNumbers_;
	Transform tHud_;
	int Num_[3];
	std::string kinokoNum;
	int EnemyNum_;
public:
	HUD(GameObject* parent);
	

	std::vector<Transform> tNumber;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

