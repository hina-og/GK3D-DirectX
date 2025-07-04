#pragma once
#include "Button.h"
#include "Engine/CsvReader.h"
#include "Engine/Text.h"
#include "Animation.h"
#include "Puppet.h"
#include "RangeView.h"



class PuppetStorage
	: public GameObject
{
	//�L�����̐������̉��s���镶����
	const int CHARA_TEXT_WIDTH_NUM{ 8 };

	//PuppetStorageData.csv�̍s
	enum IMAGE_ROW
	{
		TABLE = 1,
		PUPPET,
		PUPPET_NUM,
		STATUS,
		HP,
		POWER,
		SPEED,
		RANGE_VIEW,
		PUPPET_TEXT,
	};

	//�L�����̃X�e�[�^�X�\���̈ʒu
	struct statusViewPosition
	{
		XMFLOAT3
			base,//�x�[�X
			hp,//HP
			power,//�U����
			speed,//���x
			rangeView,//�U���͈�
			text;//�L�����̐�����
	};

	struct Storage
	{
		int num;//�����Ă���L�����̐�
		int type;//�L�����̎��
		int x, y;//�\���ʒu
		Button button;//�{�^��
		std::string name;//�L�����̖��O
		Text numText;//�����Ă���L�����̐��̃e�L�X�g
		int textX, textY;//�e�L�X�g�̈ʒu
		Puppet* puppet;//�X�e�[�^�X�擾�̂��߂̃|�C���^
		bool isMade;//1�ȏ�������
	};
	
	//�n���h��
	int hTable_;//���̖؂̔̉摜
	int hSelectFrame_;//�I�����Ă���L�����̖ڈ�̉摜
	int hStatusBase_;//�X�e�[�^�X�\���̃x�[�X�̉摜

	Animation addAnim_;//�L�������ǉ����ꂽ���̃A�j���[�V����

	//�X�e�[�^�X�\���֌W
	statusViewPosition viewPos_;//��ʒu
	Text* hpText_;//HP�̃e�L�X�g
	Text* powerText_;//�U���͂̃e�L�X�g
	Text* speedText_;//���x�̃e�L�X�g
	RangeView* rangeView_;//�U���͈͂̌�����

public:
	Storage puppetList_[CHARA_TYPE::CHARA_END];//���ꂼ��̃L�����̃X�g���[�W
	int selectPuppetNumber;//�I�����Ă���L�����̔ԍ�

	PuppetStorage(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	//�摜�ǂݍ���
	void LoadImageData(CsvReader _csv);
	//�X�g���[�W�ɃL������ǉ�����
	bool AddStorage(int _type);
};

