#include "PuppetStorage.h"
#include "Char.h"
#include "Engine/Direct3D.h"
#include "MaterialTable.h"
#include "PuppetFactory.h"

PuppetStorage::PuppetStorage(GameObject* parent)
	:GameObject(parent, "PuppetStorage")
{
}

void PuppetStorage::Initialize()
{
	selectPuppetNumber = 0;

	addAnim_.Initialize("Image\\flashAnim.png", 0, 0, 64, 64, false, 3, false);
	addAnim_.SetSpeed(0.15);

	selectFrame_ = Image::Load("Image\\frame.png");
	assert(selectFrame_ >= 0);

	hpText_ = new Text;
	hpText_->Initialize();
	powerText_ = new Text;
	powerText_->Initialize();
	speedText_ = new Text;
	speedText_->Initialize();

	rangeView_ = new RangeView();
	rangeView_->Initialize({0.5,-0.5,0});
}

void PuppetStorage::Update()
{
	for (int i = 0;i < CHARA_TYPE::CHARA_END;i++)
	{
		puppetList_[i].button.Update();

		if (puppetList_[i].button.isDown_)
		{
			Image::SetPosition(selectFrame_, { (float)puppetList_[selectPuppetNumber].x / Direct3D::screenWidth_,(float)puppetList_[selectPuppetNumber].y / -Direct3D::screenHeight_ ,0 });
		}
		if (puppetList_[i].button.isPress_)
		{
			selectPuppetNumber = i;
		}
	}


	addAnim_.Update();
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

		puppetList_[i].puppet = CreatePuppetByName(puppetList_[i].name, this);
		puppetList_[i].puppet->Initialize();
	}

	addAnim_.Draw();
	Image::Draw(selectFrame_);
	//Image::Draw(hStatusBase_);
	
	for (int i = 0; i < CHARA_TYPE::CHARA_END; i++)
	{
		if (puppetList_[i].button.isPress_)
		{
			hpText_->Draw(1700, 500, puppetList_[selectPuppetNumber].puppet->GetHitPoint());
			powerText_->Draw(1700, 560, puppetList_[selectPuppetNumber].puppet->GetPower());
			speedText_->Draw(1700, 620, puppetList_[selectPuppetNumber].puppet->GetSpeed());

			rangeView_->SetData(puppetList_[selectPuppetNumber].puppet->range_);
			rangeView_->Draw();
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

	hStatusBase_ = Image::Load("Image\\" + _csv.GetString(13, name) + ".png");
	assert(hStatusBase_ >= 0);
	Image::SetPosition(hStatusBase_, { _csv.GetFloat(13,posX) / Direct3D::screenWidth_,_csv.GetFloat(13,posY) / Direct3D::screenHeight_,0 });

	Image::SetPosition(selectFrame_, { (float)puppetList_[0].x / Direct3D::screenWidth_,(float)puppetList_[0].y / -Direct3D::screenHeight_ ,0 });
}

bool PuppetStorage::AddStorage(int _type)
{
	if (_type >= 0)
	{
		addAnim_.SetPosition({ (float)puppetList_[_type].x,(float)puppetList_[_type].y,0 });
		addAnim_.Start();

		puppetList_[_type].num++;
		return true;
	}
	return false;
}
