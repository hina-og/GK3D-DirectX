#include "PuppetStorage.h"
#include "Char.h"
#include "Engine/Direct3D.h"
#include "MaterialTable.h"

PuppetStorage::PuppetStorage(GameObject* parent)
	:GameObject(parent, "PuppetStorage")
{
}

void PuppetStorage::Initialize()
{
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
		puppetList_[i].numText.Draw(puppetList_[i].x + puppetList_[i].textX, puppetList_[i].y + puppetList_[i].textY, puppetList_[i].num);
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
	hTable_ = Image::Load("Image\\"+_csv.GetString(3,name) + ".png");
	assert(hTable_ >= 0);
	transform_.scale_ = { _csv.GetFloat(3,scaleX),_csv.GetFloat(3,scaleY),0 };
	transform_.position_ = { _csv.GetFloat(3,posX),_csv.GetFloat(3,posY),0 };
	Image::SetTransform(hTable_, transform_);


	std::string puppetName_[CHARA_TYPE::CHARA_END]
	{
		"Mouse",
		"Zombie",
		"Mushroom",
		"Slime",
		"Golem",
		"Ghost",
	};

	for (int i = 0; i < CHARA_TYPE::CHARA_END; i++)
	{
		puppetList_[i].name = puppetName_[i];
		std::string fileName_ = "Image\\" + puppetList_[i].name + ".png";
		puppetList_[i].button.LoadButtonImage(fileName_);

		
		puppetList_[i].type = i;
		puppetList_[i].x = i % 4 * puppetList_[i].button.GetSize().x + _csv.GetFloat(6, posX);
		puppetList_[i].y = i / 4 * puppetList_[i].button.GetSize().y + _csv.GetFloat(6, posY);
		puppetList_[i].button.Initialize(puppetList_[i].x, puppetList_[i].y, fileName_);
		puppetList_[i].num = 0;
		puppetList_[i].numText.Initialize();
		puppetList_[i].textX = _csv.GetFloat(7, posX);
		puppetList_[i].textY = _csv.GetFloat(7, posY);
	}

}

bool PuppetStorage::AddStorage(int _type)
{
	if (_type >= 0)
	{
		puppetList_[_type].num++;
		return true;
	}
	return false;
}
