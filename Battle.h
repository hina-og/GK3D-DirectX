#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include "MaterialTable.h"
#include "HUD.h"

class Battle
	: public GameObject
{
	const int GiveMaterialTime{ 3 };

	enum StageDataLine
	{
		STAGE_DATA = 0,//�X�e�[�W�̊�{���̗�
		SPAWN_DATA,
	};

	//�X�e�[�W�̊�{���
	enum StageData
	{
		TIME = 0,//����
		HP,//�̗�
		GIVE_MATERIAL_NUM,//�^������f�ނ̐�
	};

	enum SpawnDataRow
	{
		SPAWN_NAME = 0,
		SPAWN_TIME,
		SPAWN_LINE,
	};

	struct SpawnData
	{
		int type_;//�o�Ă���L�����̎��
		float time_;//�o�Ă���c�莞��
		int line_;//�o�Ă����
	};
	std::vector<SpawnData> spawnList_;
	int spawnedNum;
	
	int hBGM_;

	bool isTimeStart;

	float getMaterialTime;

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
	Battle(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	void SetSelectTile(XMFLOAT3 _selectPos);
	XMFLOAT3 GetSelectTile();
};

