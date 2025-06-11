#include "HUD.h"
#include "Engine/Image.h"
#include "Engine/CsvReader.h"
#include "Engine/Time.h"

HUD::HUD(GameObject* parent)
	: GameObject(parent, "HUD")
{
}

void HUD::Initialize()
{
	CsvReader ImageData;
	ImageData.Load("GameData\\ImageData.csv");

	hBaseHitPoint_ = Image::Load("Image\\BaseHitPoint.png");
	assert(hBaseHitPoint_ >= 0);

	hHitPoint_ = Image::Load("Image\\HitPoint.png");
	assert(hHitPoint_ >= 0);

	hTimeBase_ = Image::Load("Image\\TimeBase.png");
	assert(hTimeBase_ >= 0);

	timeText = new Text;
	timeText->Initialize();

	for (int line = 1;line < ImageData.GetLines();line++)
	{

		if (ImageData.GetString(line, 0) == "HP")
		{
			transformHP_.position_ = { ImageData.GetFloat(line,1),ImageData.GetFloat(line,2),0 };
			Image::SetTransform(hBaseHitPoint_, transformHP_);
			Image::SetTransform(hHitPoint_, transformHP_);
		}
		if (ImageData.GetString(line, 0) == "TimeBase")
		{
			transformTime_.position_ = { ImageData.GetFloat(line,1),ImageData.GetFloat(line,2),0 };
			Image::SetTransform(hTimeBase_, transformTime_);
		}
		if (ImageData.GetString(line, 0) == "TimeText")
		{
			transformTimeText_.position_ = { ImageData.GetFloat(line,1),ImageData.GetFloat(line,2),0 };
		}
	}

	HP_ = maxHP;
}

void HUD::Update()
{
	Transform hpTransfofm_ = transformHP_;

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
}

void HUD::Release()
{
}

void HUD::InitHP(int _hp)
{
	maxHP = _hp;
	HP_ = maxHP;
}

void HUD::TimeUpdate()
{
	time_ -= Time::GetDeltaTime();
}

std::string HUD::ToMinutesString()
{
	int minutes = (int)time_ / 60;
	int seconds = (int)time_ % 60;

	std::string secStr;
	if (seconds < 10) {
		// —á‚¦‚Î•b‚ª5‚È‚ç "05" ‚É‚·‚é
		secStr = "0" + std::to_string(seconds);
	}
	else {
		// —á‚¦‚Î•b‚ª12‚È‚ç "12" ‚Ì‚Ü‚Ü
		secStr = std::to_string(seconds);
	}

	return std::to_string(minutes) + ":" + secStr;
}
