#pragma once
#include "Animation.h"
#include "Button.h"
#include "Engine/Text.h"

class Calendar
{
	//�n���h��
	Animation hImage_;//�y�[�W���߂���A�j���[�V����
	int hPageSE_;//�y�[�W���߂��鉹
	int hNumber_;//�X�e�[�W�̐���


	int day_;
	Text* tutoerialText_;
public:
	Button calendar_;

	Calendar();
	void Initialize();
	void Update();
	void Draw();
	//�O�ɍs��
	void Back();
	//���ɍs��
	void Next();
	void SetDay(int _day);
};

