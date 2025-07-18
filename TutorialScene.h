#pragma once
#include "Engine/GameObject.h"
#include "Stage.h"
#include "Player.h"
#include "Enemy.h"
#include "MaterialTable.h"
#include "HUD.h"

//�G���o�Ă��鎞��
const float ENEMY_SPAWN_INTERVAL{ 10.0f };

class TutorialScene :
    public GameObject
{
	//BGM�n���h��
	int hBGM_;
	//���Ԃ�������
	bool isTimeStart;
	//�L�����̌���
	int selectDir;

	/*�|�C���^*/
	Stage* stage;//�X�e�[�W
	Player* player;//�v���C���[�̑���֌W
	Enemy* enemy;//�G�֌W
	MaterialTable* material;//�f�ފ֌W
	HUD* hud;//�̗͂Ƃ����ԂƂ��̕\��

	//�I�����Ă���^�C��
	XMFLOAT3 selectPos_;

	bool isReady;
	bool isBattleEnd;
public:
    TutorialScene(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};
