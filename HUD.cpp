#include "HUD.h"
#include "Engine/Image.h"
#include "Engine/CsvReader.h"
#include "Engine/Time.h"
#include "ImageDataUtil.h"

HUD::HUD(GameObject* parent)
	: GameObject(parent, "HUD")
{
}

void HUD::Initialize()
{
	//csvで位置やサイズの設定
	CsvReader csv;
	csv.Load("ImageData\\HUDData.csv");

	hBaseHitPoint_ = Image::Load("Image\\" + csv.GetString(BASE_HP, NAME) + ".png");
	assert(hBaseHitPoint_ >= 0);
	hHitPoint_ = Image::Load("Image\\" + csv.GetString(HP, NAME) + ".png");
	assert(hHitPoint_ >= 0);
	transformHP_.position_ = { csv.GetFloat(BASE_HP,POSITION_X),csv.GetFloat(BASE_HP,POSITION_Y),0 };
	Image::SetTransform(hBaseHitPoint_, transformHP_);
	Image::SetTransform(hHitPoint_, transformHP_);


	hTimeBase_ = Image::Load("Image\\" + csv.GetString(BASE_TIME, NAME) + ".png");
	assert(hTimeBase_ >= 0);
	transformTime_.position_ = { csv.GetFloat(BASE_TIME,POSITION_X),csv.GetFloat(BASE_TIME,POSITION_Y),0 };
	Image::SetTransform(hTimeBase_, transformTime_);

	timeText = new Text;
	timeText->Initialize();
	transformTimeText_.position_ = { csv.GetFloat(TIME,POSITION_X),csv.GetFloat(TIME,POSITION_Y),0 };

	hModelDirection_ = Image::Load("Image\\ModelDirection.png");
	assert(hModelDirection_ >= 0);

	Transform fTrans;
	fTrans.position_ = { csv.GetFloat(DIRECTION,POSITION_X),csv.GetFloat(DIRECTION,POSITION_Y),0 };
	fTrans.scale_ = { csv.GetFloat(DIRECTION, SCALE_X), csv.GetFloat(DIRECTION, SCALE_Y), 0 };
	Image::SetTransform(hModelDirection_, fTrans);

	isTutorial = false;
}

void HUD::Update()
{
	Transform hpTransfofm_ = transformHP_;
	//HPが減ったらその分緑の部分を短くして左にずらす
	hpTransfofm_.position_.x = hpTransfofm_.position_.x + Image::GetImageSize(hHitPoint_).x - Image::GetImageSize(hBaseHitPoint_).x;
	hpTransfofm_.scale_.x = (float)HP_ / (float)maxHP;
	Image::SetTransform(hHitPoint_, hpTransfofm_);
}

void HUD::Draw()
{
	Image::Draw(hBaseHitPoint_);
	Image::Draw(hHitPoint_);
	Image::Draw(hTimeBase_);

	timeText->Draw((int)transformTimeText_.position_.x, transformTimeText_.position_.y, ToMinutesString().c_str());

	Image::Draw(hModelDirection_);
}

void HUD::Release()
{
}

void HUD::TutorialInitialize()
{
	maxHP = TUTORIAL_HP;
	HP_ = maxHP;
	time_ = 0.0f;
	isTutorial = true;
}

void HUD::InitHP(int _hp)
{
	maxHP = _hp;
	HP_ = maxHP;
}

void HUD::AddHP(int _hp)
{

}

void HUD::TimeUpdate()
{
	time_ -= Time::GetDeltaTime();
}

std::string HUD::ToMinutesString()
{
	int minutes = (int)time_ / 60;//秒
	int seconds = (int)time_ % 60;//分

	std::string secStr;

	//10分未満の場合は10の位に0を入れる
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
	int imageSize = Image::GetImageSize(hModelDirection_).y;//正方形だから縦幅だけ取得する
	Image::SetRect(hModelDirection_, _dir * imageSize, 0, imageSize, imageSize);
}
