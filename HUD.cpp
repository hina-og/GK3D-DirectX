#include "HUD.h"
#include "Engine/Image.h"
#include "Engine/CsvReader.h"

HUD::HUD(GameObject* parent)
	: GameObject(parent, "HUD")
{
}

void HUD::Initialize()
{
	CsvReader ImageData;
	ImageData.Load("GameData\\ImageData.csv");

	hBaceHitPoint_ = Image::Load("Image\\BaseHitPoint.png");
	assert(hBaceHitPoint_ >= 0);

	hHitPoint_ = Image::Load("Image\\HitPoint.png");
	assert(hHitPoint_ >= 0);

	HP_ = maxHP;
	for (int line = 0;line < ImageData.GetLines();line++)
	{
		if (ImageData.GetString(line, 0) == "HP")
		{
			transform_.position_ = { ImageData.GetFloat(line,1),ImageData.GetFloat(line,2),0};
			break;
		}
	}
	Image::SetTransform(hBaceHitPoint_, transform_);
	Image::SetTransform(hHitPoint_, transform_);
}

void HUD::Update()
{
	Transform hpTransfofm_ = transform_;

	hpTransfofm_.position_.x = transform_.position_.x + Image::GetImageSize(hHitPoint_).x - Image::GetImageSize(hBaceHitPoint_).x;
	hpTransfofm_.scale_.x = (float)HP_ / (float)maxHP;
	Image::SetTransform(hHitPoint_, hpTransfofm_);
}

void HUD::Draw()
{
	
	Image::Draw(hBaceHitPoint_);
	Image::Draw(hHitPoint_);
}

void HUD::Release()
{
}

void HUD::InitHP(int _hp)
{
	maxHP = _hp;
	HP_ = maxHP;
}
