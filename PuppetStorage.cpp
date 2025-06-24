#include "PuppetStorage.h"
#include "Char.h"
#include "Engine/Direct3D.h"
#include "MaterialTable.h"
#include "PuppetFactory.h"
#include "ImageDataUtil.h"

PuppetStorage::PuppetStorage(GameObject* parent)
	:GameObject(parent, "PuppetStorage")
{
}

void PuppetStorage::Initialize()
{
	CsvReader csv("ImageData\\PuppetStorageData.csv");



	selectPuppetNumber = 0;

	addAnim_.Initialize("Image\\flashAnim.png",
		0, 0,      // 開始位置
		64, 64,    // サイズ
		false,     // 繰り返すか
		3,         // フレーム数
		false      // 終了後も描画するか
	);	
	addAnim_.SetSpeed(0.15);

	selectFrame_ = Image::Load("Image\\frame.png");
	assert(selectFrame_ >= 0);

	rangeView_ = new RangeView();

	LoadImageData(csv);

	hpText_ = new Text;
	hpText_->Initialize();
	powerText_ = new Text;
	powerText_->Initialize();
	speedText_ = new Text;
	speedText_->Initialize();

	

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
	hTable_ = Image::Load("Image\\"+_csv.GetString(TABLE,NAME) + ".png");
	assert(hTable_ >= 0);
	transform_.scale_ = { _csv.GetFloat(TABLE,SCALE_X),_csv.GetFloat(TABLE,SCALE_Y),0 };
	transform_.position_ = { _csv.GetFloat(TABLE,POSITION_X),_csv.GetFloat(TABLE,POSITION_Y),0 };
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
		puppetList_[i].x = i % 4 * puppetList_[i].button.GetSize().x + _csv.GetFloat(PUPPET, POSITION_X);
		puppetList_[i].y = i / 4 * puppetList_[i].button.GetSize().y + _csv.GetFloat(PUPPET, POSITION_Y);
		puppetList_[i].button.Initialize(puppetList_[i].x, puppetList_[i].y, fileName_);
		puppetList_[i].num = 0;
		puppetList_[i].numText.Initialize();
		puppetList_[i].textX = _csv.GetFloat(PUPPETNUM, POSITION_X);
		puppetList_[i].textY = _csv.GetFloat(PUPPETNUM, POSITION_Y);
	}

	hStatusBase_ = Image::Load("Image\\" + _csv.GetString(STATUS, NAME) + ".png");
	assert(hStatusBase_ >= 0);
	XMFLOAT3 statusPos = { _csv.GetFloat(STATUS,POSITION_X) / Direct3D::screenWidth_,_csv.GetFloat(STATUS,POSITION_Y) / Direct3D::screenHeight_,0 };
	Image::SetPosition(hStatusBase_, statusPos);
	rangeView_->Initialize(statusPos);


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
