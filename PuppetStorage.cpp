#include "PuppetStorage.h"
#include "Char.h"
#include "Engine/Direct3D.h"

PuppetStorage::PuppetStorage(GameObject* parent)
	:GameObject(parent, "PuppetStorage")
{
}

void PuppetStorage::Initialize()
{
	hTable_ = Image::Load("Image\\MaterialTable.png");
	assert(hTable_ >= 0);

	transform_.scale_ = { 0.8,1.0,1.0 };
	Image::SeScale(hTable_, transform_.scale_);
	transform_.position_ = { (float)Direct3D::screenWidth_ - Image::GetImageSize(hTable_).x,(float)Direct3D::screenHeight_ - Image::GetImageSize(hTable_).y,0 };
	Image::SetTransform(hTable_, transform_);

	std::string puppetName_[CHARA_TYPE::CHARA_END]
	{
		"Mouse",
		"Zombie",
		"Mushroom",
		"Slime",
		"Golem",
		"Ghost",
		//"Failure"
	};

	for (int i = 0;i < CHARA_TYPE::CHARA_END;i++)
	{
		puppetList_[i].num = 0;
		puppetList_[i].type = i;
		puppetList_[i].x = i % 5 * 128 + transform_.position_.x - 256;
		puppetList_[i].y = i / 5 * 128 - Image::GetImageSize(hTable_).y / 2 - transform_.position_.y;
		puppetList_[i].name = puppetName_[i];
		puppetList_[i].numText.Initialize();

		std::string fileName_ = "Image\\" + puppetList_[i].name + ".png";
		puppetList_[i].button.Initialize(puppetList_[i].x, puppetList_[i].y, fileName_);
	}

	selectPuppetNumber = 0;
}

void PuppetStorage::Update()
{
	for (int i = 0;i < CHARA_TYPE::CHARA_END;i++)
	{
		puppetList_[i].button.Update();
	}

	for (int i = 0; i < CHARA_TYPE::CHARA_END; i++)
	{
		if (puppetList_[i].button.isPress_)
		{
			selectPuppetNumber = i;
			break;
		}
	}
}

void PuppetStorage::Draw()
{
	Image::Draw(hTable_);

	for (int i = 0;i < CHARA_TYPE::CHARA_END;i++)
	{
		puppetList_[i].button.Draw();
		puppetList_[i].numText.Draw(puppetList_[i].x + transform_.position_.x - 180, puppetList_[i].y + transform_.position_.y + 80, puppetList_[i].num);
		if (puppetList_[i].num > 0)
		{
			int i = 0;
		}
	}
}

void PuppetStorage::Release()
{
}

void PuppetStorage::LoadImageData(CsvReader _csv)
{
}

void PuppetStorage::AddStorage(int _type)
{
	puppetList_[_type].num++;
}
