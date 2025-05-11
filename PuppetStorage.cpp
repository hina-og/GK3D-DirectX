#include "PuppetStorage.h"
#include "Char.h"

PuppetStorage::PuppetStorage(GameObject* parent)
	:GameObject(parent, "PuppetStorage")
{
	int i = 0;
}

void PuppetStorage::Initialize()
{
	std::string puppetName_[CHARA_TYPE::CHARA_END]
	{
		"Mouse",
		"Zombie",
		"Ghost"
		//"Failure"
	};

	for (int i = 0;i < CHARA_TYPE::CHARA_END;i++)
	{
		puppetList_[i].num = 0;
		puppetList_[i].type = i;
		puppetList_[i].x = i % 5 * 100 + 900;
		puppetList_[i].y = i / 5 * 70 + 25;
		puppetList_[i].name = puppetName_[i];
		puppetList_[i].numText.Initialize();

		std::string fileName_ = "Image\\" + puppetList_[i].name + ".png";
		puppetList_[i].button.Initialize(puppetList_[i].x, puppetList_[i].y, fileName_);
	}
}

void PuppetStorage::Update()
{
	for (int i = 0;i < CHARA_TYPE::CHARA_END;i++)
	{
		puppetList_[i].button.Update();
	}
}

void PuppetStorage::Draw()
{
	for (int i = 0;i < CHARA_TYPE::CHARA_END;i++)
	{
		puppetList_[i].button.Draw();
		puppetList_[i].numText.Draw(puppetList_[i].x, puppetList_[i].y, puppetList_[i].num);
		if (puppetList_[i].num > 0)
		{
			int i = 0;
		}
		//Char::Draw(puppetList_[i].x + 54, puppetList_[i].y + 32, std::to_string(puppetList_[i].num));
	}

	
}

void PuppetStorage::Release()
{
}

void PuppetStorage::AddStorage(int _type)
{
	puppetList_[_type].num++;
}
