#pragma once
#include "Engine/GameObject.h"
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include "MaterialTable.h"
#include "HUD.h"

class TutorialScene :
    public GameObject
{
	struct SpawnData
	{
		int type_;//�o�Ă���L�����̎��
		float time_;//�o�Ă���c�莞��
		int line_;//�o�Ă����
	};

	int hBGM_;

	bool isTimeStart;

	int selectDir;

	Stage* stage;
	Player* player;
	Enemy* enemy;
	MaterialTable* material;
	HUD* hud;

	int mouseX;
	int mouseY;
	XMFLOAT3 selectPos_;

	std::vector<Puppet*> pPuppet_;
	std::vector<Puppet*> ePuppet_;

	bool isReady;
	bool isBattleEnd;
public:
    TutorialScene(GameObject* parent);
    void Initialize();
    void Update();
    void Draw();
    void Release();
};
