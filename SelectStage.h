#pragma once
#include "Engine/GameObject.h"
#include "Calendar.h"

class SelectStage
	: public GameObject
{
	int stageNum_;//�X�e�[�W�̍ő吔
	int nowStage_;//���ݑI�����Ă���X�e�[�W

	Calendar calendar;
public:
	bool decision_;//����

	SelectStage(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();
	//�X�e�[�W���̃Z�b�^�[
	void SetStageNum(int _num);
	//�X�e�[�W���̃Q�b�^�[
	int GetStageNum();
};

