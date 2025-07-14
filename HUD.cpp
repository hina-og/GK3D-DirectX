#include "HUD.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/CsvReader.h"
#include "Engine/Time.h"
#include "ImageDataUtil.h"

HUD::HUD(GameObject* parent)
	: GameObject(parent, "HUD")
{
}

void HUD::Initialize()
{
	//csv�ňʒu��T�C�Y�̐ݒ�
	CsvReader csv;
	csv.Load("ImageData\\HUDData.csv");

	Transform fTrans;
	hBaseHitPoint_ = Image::Load("Image\\" + csv.GetString(BASE_HP, NAME) + ".png");
	assert(hBaseHitPoint_ >= 0);
	hHitPoint_ = Image::Load("Image\\" + csv.GetString(HP, NAME) + ".png");
	assert(hHitPoint_ >= 0);
	transformHP_.position_ = { csv.GetFloat(BASE_HP,POSITION_X),csv.GetFloat(BASE_HP,POSITION_Y),0 };
	Image::SetTransform(hBaseHitPoint_, transformHP_);
	Image::SetTransform(hHitPoint_, transformHP_);


	hTimeBase_ = Image::Load("Image\\" + csv.GetString(BASE_TIME, NAME) + ".png");
	assert(hTimeBase_ >= 0);
	fTrans.position_ = { csv.GetFloat(BASE_TIME,POSITION_X),csv.GetFloat(BASE_TIME,POSITION_Y),0 };
	Image::SetTransform(hTimeBase_, fTrans);

	timeText_ = new Text;
	timeText_->Initialize();
	transformTimeText_.position_ = { csv.GetFloat(TIME,POSITION_X),csv.GetFloat(TIME,POSITION_Y),0 };

	hModelDirection_ = Image::Load("Image\\ModelDirection.png");
	assert(hModelDirection_ >= 0);

	
	fTrans.position_ = { csv.GetFloat(DIRECTION,POSITION_X),csv.GetFloat(DIRECTION,POSITION_Y),0 };
	fTrans.scale_ = { csv.GetFloat(DIRECTION, SCALE_X), csv.GetFloat(DIRECTION, SCALE_Y), 0 };
	Image::SetTransform(hModelDirection_, fTrans);

	hEnemyNumBace_ = Image::Load("Image\\enemyNum.png");
	assert(hEnemyNumBace_ >= 0);
	fTrans.position_ = { csv.GetFloat(ENEMY_NUM_BACE,POSITION_X),csv.GetFloat(ENEMY_NUM_BACE,POSITION_Y),0 };
	Image::SetTransform(hEnemyNumBace_, fTrans);

	enemyNumText_ = new Text;
	enemyNumText_->Initialize();
	enemyNumTextPos_ = { csv.GetFloat(ENEMY_NUM,POSITION_X),csv.GetFloat(ENEMY_NUM,POSITION_Y),0 };

	isTutorial_ = false;

	endText_ = new Text;
	endText_->Initialize();
	endTextPos_ = { csv.GetFloat(END_TEXT,POSITION_X),csv.GetFloat(END_TEXT,POSITION_Y),0 };

	resetText_ = new Text;
	resetText_->Initialize();
	resetTextPos_ = { csv.GetFloat(MENU_TEXT,POSITION_X),csv.GetFloat(MENU_TEXT,POSITION_Y),0 };


}

void HUD::Update()
{
	Transform hpTransfofm_ = transformHP_;
	//HP���������炻�̕��΂̕�����Z�����č��ɂ��炷
	hpTransfofm_.position_.x = hpTransfofm_.position_.x + Image::GetImageSize(hHitPoint_).x - Image::GetImageSize(hBaseHitPoint_).x;
	hpTransfofm_.scale_.x = (float)HP_ / (float)maxHP_;
	Image::SetTransform(hHitPoint_, hpTransfofm_);
}

void HUD::Draw()
{
	//�摜�E�e�L�X�g�\���n
	Image::Draw(hBaseHitPoint_);
	Image::Draw(hHitPoint_);
	Image::Draw(hTimeBase_);
	Image::Draw(hModelDirection_);

	timeText_->Draw((int)transformTimeText_.position_.x, transformTimeText_.position_.y, ToMinutesString().c_str());
	

	if (isTutorial_)
	{
		//�I������̕\��
		endText_->Draw(endTextPos_.x, endTextPos_.y, "END   : E");
		//�X�e�[�W���Z�b�g����̕\��
		resetText_->Draw(resetTextPos_.x, resetTextPos_.y, "RESET : R");
	}
	else
	{
		//�`���[�g���A���ł͓G�������ɏo�邽�ߒʏ�X�e�[�W�̂ݓG�̐��\��
		Image::Draw(hEnemyNumBace_);
		enemyNumText_->Draw(enemyNumTextPos_.x, enemyNumTextPos_.y, enemyNum_);
	}
}

void HUD::Release()
{
}

void HUD::TutorialInitialize()
{
	maxHP_ = TUTORIAL_HP;
	HP_ = maxHP_;
	//time_ = 5.0f;
	isTutorial_ = true;
}

void HUD::InitHP(int _hp)
{
	maxHP_ = _hp;
	HP_ = maxHP_;
}

void HUD::AddHP(int _hp)
{
	HP_ += _hp;
}

void HUD::TimeUpdate()
{
	time_ -= Time::GetDeltaTime();
}

std::string HUD::ToMinutesString()
{
	int minutes = (int)time_ / 60;//�b
	int seconds = (int)time_ % 60 + 1;//���i0���傫����1��菬�����ꍇ��1�b�ƕ\��������������+1���Ă�j

	std::string secStr;

	//10�������̏ꍇ��10�̈ʂ�0������
	if (seconds < 10) 
	{
		secStr = "0" + std::to_string(seconds);
	}
	else 
	{
		secStr = std::to_string(seconds);
	}

	return std::to_string(minutes) + ":" + secStr;
}

void HUD::SetDirection(int _dir)
{
	int imageSize = Image::GetImageSize(hModelDirection_).y;//�����`������c�������擾����
	Image::SetRect(hModelDirection_, _dir * imageSize, 0, imageSize, imageSize);
}

void HUD::SetEnemyNum(int _enemyNum)
{
	enemyNum_ = _enemyNum;
}
