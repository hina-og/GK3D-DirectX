#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include "MaterialTable.h"
#include "HUD.h"

class PlayScene
	: public GameObject
{
	//�f�ނ����炦��b���i����    3�b  ��1����炦��j
	const float GiveMaterialTime{ 3.0f };

	enum StageDataLine
	{
		STAGE_DATA = 0,//�X�e�[�W�̊�{���̗�
		SPAWN_DATA,//�G�̏o�����̍ŏ��̗�
	};

	//�X�e�[�W�̊�{���icsv�t�@�C����1�s�ځj
	enum StageData
	{
		TIME = 0,//����
		HP,//�̗�
		GIVE_MATERIAL_NUM,//�^������f�ނ̐�
	};

	//�G�̏o�����icsv�t�@�C����2�s�ڈȍ~�j
	enum SpawnDataRow
	{
		SPAWN_NAME = 0,//�G�̖��O
		SPAWN_TIME,//�o�����鎞��
		SPAWN_LINE,//�o������ʒu
	};

	//�I������
	enum END_CONDITIONS
	{
		All_Kill,//�G�����Ȃ�
		Time_Up,//�Ō�܂Ŏ����
		Dead,//�̗͂��Ȃ��Ȃ���
		END,//�I�������̌�
		//���ǉ�����ꍇ�͐�΂�END���Ō�ɂ���
	};

	struct SpawnData
	{
		int type_;//�o�Ă���L�����̎��
		float time_;//�o�Ă���c�莞��
		int line_;//�o�Ă����
	};
	//�G�o�����X�g
	std::vector<SpawnData> spawnList_;
	//�X�|�[�������G�̐�
	int spawnedNum;
	//BGM�̃n���h��
	int hBGM_;
	//���Ԃ������Ă��������i���[�h���������Ă��瓮�������߁j
	bool isTimeStart;
	//�f�ޓ���̃^�C�}�[
	float getMaterialTime;
	//�I�����Ă������
	int selectDir;

	//---�e�I�u�W�F�N�g�̃|�C���^---
	Stage* stage;
	Player* player;
	Enemy* enemy;
	MaterialTable* material;
	HUD* hud;
	//------------------------------


	//�v���C���[�̃L�����̔z��
	std::vector<Puppet*> pPuppet_;
	//�G�̃L�����̔z��
	std::vector<Puppet*> ePuppet_;

	//�����ł�����
	bool isReady;
	//�o�g�����I�������
	bool isBattleEnd;
public:
	PlayScene(GameObject* parent);
	//������
	void Initialize() override;
	//�X�V
	void Update() override;
	//�`��
	void Draw() override;
	//�J��
	void Release() override;
};

