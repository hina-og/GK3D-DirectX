#include "HUD.h"
#include "Engine/Image.h"
#include "Engine/Debug.h"

HUD::HUD(GameObject* parent)
	:GameObject(parent, "HUD"), hHUD_(-1)
{
}

void HUD::Initialize()
{
	hHUD_ = Image::Load("Image\\HUD.png");
	assert(hHUD_ >= 0);
	hNumbers_ = Image::Load("Image\\number.png");
	assert(hNumbers_ >= 0);
}

void HUD::Update()
{
	tHud_.scale_ = { 0.8,0.8,0.8 };
	tHud_.position_ = { -0.68,0.82,0 };
	//kinokoNum = std::to_string(((PlayScene*)GetParent())->GetEnemyNum());
}

void HUD::Draw()
{
	Image::SetTransform(hHUD_, tHud_);
	Image::Draw(hHUD_);

	

	int j = 0;
	for (int i = 0; i < kinokoNum.length(); i++)
	{
		//if (((PlayScene*)GetParent())->GetEnemyNum() > 99)
		//{
		//	Num_[j] = kinokoNum[i] - '0';
		//	j++;
		//}
		//else
		//{
		//	Num_[0] = 0;
		//	j++;
		//	Num_[j] = kinokoNum[i] - '0';
		//}
	}

	Transform trans1;
	trans1.scale_ = { 0.9,0.4,1.0 };
	trans1.position_ = { -0.73,0.82,0 };

	Image::SetRect(hNumbers_, 51.2 * Num_[0], 0, 51.2, 168);
	Image::SetTransform(hNumbers_, trans1);
	Image::Draw(hNumbers_);

	Transform trans2;
	trans2.scale_ = { 0.9,0.4,1.0 };
	trans2.position_ = { -0.65,0.82,0 };

	Image::SetRect(hNumbers_, 51.2 * Num_[1], 0, 51.2, 168);
	Image::SetTransform(hNumbers_, trans2);
	Image::Draw(hNumbers_);

	Transform trans3;
	trans3.scale_ = { 0.9,0.4,1.0 };
	trans3.position_ = { -0.57,0.82,0 };

	Image::SetRect(hNumbers_, 51.2 * Num_[2], 0, 51.2, 168);
	Image::SetTransform(hNumbers_, trans3);
	Image::Draw(hNumbers_);

	
}

void HUD::Release()
{
}
