#include "PuppetStorage.h"
#include "Engine/Direct3D.h"
#include "MaterialTable.h"
#include "PuppetFactory.h"
#include "ImageDataUtil.h"
#include "Font.h"

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
	float animationSpeed = 0.15;
	addAnim_.SetSpeed(animationSpeed);

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

	Font::Initialize("Font/keinan_pop/FontConfig.txt");

}

void PuppetStorage::Update()
{
	for (int i = 0;i < CHARA_TYPE::CHARA_END;i++)
	{
		puppetList_[i].button.Update();

		if (puppetList_[i].button.isPress_)
		{
			Image::SetPosition(selectFrame_, { (float)puppetList_[selectPuppetNumber].x / Direct3D::screenWidth_,(float)puppetList_[selectPuppetNumber].y / -Direct3D::screenHeight_ ,0 });
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

	
	for (int i = 0; i < CHARA_TYPE::CHARA_END; i++)
	{
		if (puppetList_[i].button.isPress_)
		{
			if (puppetList_[i].isMade)
			{
				Image::Draw(hStatusBase_);
				rangeView_->SetData(puppetList_[selectPuppetNumber].puppet->range_);
				rangeView_->Draw();
				hpText_->Draw(viewPos_.hp.x, viewPos_.hp.y, puppetList_[selectPuppetNumber].puppet->GetHitPoint());
				powerText_->Draw(viewPos_.power.x, viewPos_.power.y, puppetList_[selectPuppetNumber].puppet->GetPower());
				speedText_->Draw(viewPos_.speed.x, viewPos_.speed.y, puppetList_[selectPuppetNumber].puppet->GetSpeed(), 1);
			}
			else
			{
				XMFLOAT3 textSize = { 0.3f,0.3f,1.0f };
				Font::Draw("Hiragana", viewPos_.text.x, viewPos_.text.y, puppetList_[selectPuppetNumber].puppet->GetText(), textSize, CHARA_TEXT_WIDTH_NUM);
			}
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

	std::string puppetName_[CHARA_TYPE::CHARA_END];

	for (int i = 0; i < CHARA_TYPE::CHARA_END; i++)
	{
		puppetName_[i] = GetCharacterNameFromType(i);
		puppetList_[i].name = puppetName_[i];
		std::string fileName_ = "Image\\" + puppetList_[i].name + ".png";
		puppetList_[i].button.LoadButtonImage(fileName_);

		int expansionNum = 4;
		puppetList_[i].type = i;
		puppetList_[i].x = i % expansionNum * puppetList_[i].button.GetSize().x + _csv.GetFloat(PUPPET, POSITION_X);
		puppetList_[i].y = i / expansionNum * puppetList_[i].button.GetSize().y + _csv.GetFloat(PUPPET, POSITION_Y);
		puppetList_[i].button.Initialize(puppetList_[i].x, puppetList_[i].y, fileName_);
		puppetList_[i].num = 0;
		puppetList_[i].numText.Initialize();
		puppetList_[i].textX = _csv.GetFloat(PUPPET_NUM, POSITION_X);
		puppetList_[i].textY = _csv.GetFloat(PUPPET_NUM, POSITION_Y);
	}

	hStatusBase_ = Image::Load("Image\\" + _csv.GetString(STATUS, NAME) + ".png");
	assert(hStatusBase_ >= 0);
	viewPos_.base = { _csv.GetFloat(STATUS,POSITION_X) / Direct3D::screenWidth_,_csv.GetFloat(STATUS,POSITION_Y) / Direct3D::screenHeight_,0 };
	Image::SetPosition(hStatusBase_, viewPos_.base);

	viewPos_.hp = { _csv.GetFloat(HP,POSITION_X),_csv.GetFloat(HP,POSITION_Y),0 };
	viewPos_.power = { _csv.GetFloat(POWER,POSITION_X),_csv.GetFloat(POWER,POSITION_Y),0 };
	viewPos_.speed = { _csv.GetFloat(SPEED,POSITION_X),_csv.GetFloat(SPEED,POSITION_Y),0 };

	viewPos_.rangeView = { _csv.GetFloat(RANGE_VIEW,POSITION_X),_csv.GetFloat(RANGE_VIEW,POSITION_Y),0 };
	rangeView_->Initialize(viewPos_.rangeView);

	viewPos_.text = { _csv.GetFloat(PUPPET_TEXT,POSITION_X),_csv.GetFloat(PUPPET_TEXT,POSITION_Y),0 };

	Image::SetPosition(selectFrame_, { (float)puppetList_[0].x / Direct3D::screenWidth_,(float)puppetList_[0].y / -Direct3D::screenHeight_ ,0 });

}

bool PuppetStorage::AddStorage(int _type)
{
	if (_type >= 0)
	{
		addAnim_.SetPosition({ (float)puppetList_[_type].x,(float)puppetList_[_type].y,0 });
		addAnim_.Start();
		puppetList_[_type].isMade = true;
		puppetList_[_type].num++;
		return true;
	}
	return false;
}
