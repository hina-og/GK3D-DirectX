#pragma once
#include <string>
#include "Engine/GameObject.h"
#include "Engine/Text.h"

class HUD
	: public GameObject
{
	//csv�̏ォ�珇��
	enum IMAGE_ROW
	{
		BASE_HP = 1,
		HP,
		BASE_TIME,
		TIME,
		DIRECTION,
	};

	int maxHP;//�ϋv�̍ő�l

	int hHitPoint_;//�c���HP�i�΂̂Ƃ���j
	int hBaseHitPoint_;//HP�̃x�[�X�摜
	Transform transformHP_;//HP�̕\���ʒu
	
	int hTimeBase_;//���Ԃ̌��ɕ\��������
	Text* timeText;//���Ԃ̃e�L�X�g
	Transform transformTime_;//���Ԃ̌��̂�̈ʒu
	Transform transformTimeText_;//���Ԃ̃e�L�X�g�̈ʒu

	int hModelDirection_;//�L������ݒu����Ƃ��̖��
	Transform transformArrow_;//���̕\���ʒu
public:
	int HP_;
	float time_;

	HUD(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	//HP�̏�����
	void InitHP(int _hp);
	void TimeUpdate();
	//���Ԃ𕶎���ɕϊ�����
	std::string ToMinutesString();
	void SetDirection(int _dir);
};

