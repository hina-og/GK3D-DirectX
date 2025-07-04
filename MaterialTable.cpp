#include "MaterialTable.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"
#include "Engine/Direct3D.h"
#include "ImageDataUtil.h"



MaterialTable::MaterialTable(GameObject* parent)
	: GameObject(parent, "MaterialTable"), hSelect_(-1)
{
}

void MaterialTable::Initialize()
{

	CsvReader csv("ImageData\\MaterialTableData.csv");

	storage = Instantiate<PuppetStorage>(this);

	ReadRecipe();

	InitFrameTable(TABLE, csv);
	InitMaterialList(MATERIAL, csv);
	InitMakeButton(MAKE, csv);
	InitSlotFrames(MATERIAL_TABLE, csv);


	addAnim_.Initialize(FLASH);
	steamAnim_.Initialize(STEAM);

	hSelect_ = Audio::Load("Sounds\\SE\\add.wav");
	hChoise_ = Audio::Load("Sounds\\SE\\Chenge.wav", false, TABLE_SIZE);

	//�m����config.ini����ǂݍ��݁i�ǂݍ��߂Ȃ�������100������j
	returnProbability_ = GetPrivateProfileInt("Material", "return_probability ", 100, ".\\config.ini");

	quickRecipe = Instantiate<QuickRecipe>(this);
	quickRecipe->SetPosition(QUICK_RECIPE, csv);

	hPot_ = Image::Load("Image\\" + csv.GetString(POT, NAME) + ".png");
	assert(hPot_ >= 0);

	Transform tempTrans;
	tempTrans.position_ = { csv.GetFloat(POT,POSITION_X),csv.GetFloat(POT,POSITION_Y),0 };
	tempTrans.scale_ = { csv.GetFloat(POT,SCALE_X),csv.GetFloat(POT,SCALE_Y),0 };
	Image::SetTransform(hPot_, tempTrans);
	tempTrans.position_.y = -tempTrans.position_.y - Image::GetImageSize(hPot_).y / 2;
	steamAnim_.SetPosition(tempTrans.position_);
}

void MaterialTable::Update()
{
	for (int i = 0; i < materialName_.size(); i++)
	{
		materialList_[i].button.Update();
	}
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		table.material[i].button.Update();
	}
	makeButton_.Update();


	//�f�ނ�I��������e�[�u���ɒu��
	for (int i = 0; i < materialName_.size(); i++)
	{
		if (materialList_[i].button.isDown_ && table.num < TABLE_SIZE && 0 < materialList_[i].num)
		{
			table.material[table.num].type = materialList_[i].type;
			table.material[table.num].name = materialList_[i].name;
			table.material[table.num].button.ChangeImage("Image\\" + table.material[table.num].name + ".png");
			table.num++;
			materialList_[i].num--;
			Audio::Play(hChoise_);
		}
	}


	//�I�������e�[�u���̑f�ނ�߂�
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (table.material[i].button.isDown_ && 0 < table.num && table.material[i].name != "frame")
		{

			for (int j = 0; j < materialName_.size(); j++)
			{
				if (table.material[i].type == materialList_[j].type)
				{
					materialList_[j].num++;
				}
			}

			for (int j = i; j < TABLE_SIZE - 1; j++)
			{
				table.material[j].type = table.material[j + 1].type;
				table.material[j].name = table.material[j + 1].name;
				table.material[j].button.ChangeImage("Image\\" + table.material[j].name + ".png");
			}


			table.num--;

			table.material[table.num].type = MATERIAL_TYPE::EMPTY;
			table.material[table.num].name = "frame";
			table.material[table.num].button.ChangeImage("Image\\frame.png");
			Audio::Play(hChoise_);
		}
	}
	if ((makeButton_.isDown_ || Input::IsKeyDown(DIK_SPACE) ) && table.material[0].name != "frame")
	{
		TableReset();
		Audio::Play(hSelect_);
	}
	steamAnim_.Update();
	addAnim_.Update();
}

void MaterialTable::Draw()
{
	Image::Draw(hTable_);
	Image::Draw(hPot_);
	steamAnim_.Draw();

	for (int i = 0; i < materialName_.size(); i++)
	{
		materialList_[i].button.Draw();
		materialList_[i].numText.Draw(materialList_[i].buttonPosition.x + materialList_[i].textPosition.x, materialList_[i].buttonPosition.y + materialList_[i].textPosition.y, materialList_[i].num);
	}

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		table.material[i].button.Draw();
	}

	makeButton_.Draw();
	addAnim_.Draw();

	quickRecipe->Draw();
}

void MaterialTable::Release()
{
}

void MaterialTable::ReadRecipe()
{
	CsvReader csv("GameData\\Recipe.csv");

	materialName_.resize(csv.GetColumns(0) - 1);
	for (int column = 1; column < csv.GetColumns(0); column++)
	{
		materialName_[column - 1] += csv.GetString(0, column);
	}

	for (int line = 1; line < csv.GetLines(); line++)
	{
		Recipe r;
		for (int column = 1; column < csv.GetColumns(line); column++)
		{
			r.ratio.push_back(csv.GetInt(line, column));
		}
		r.puppetType = line - 1;
		recipeList_.push_back(r);
	}
}

int MaterialTable::MakePuppet()
{
	int result = -1;
	int material[MATERIAL_END];
	for (int i = 0; i < materialName_.size(); i++)
	{
		material[i] = 0;
	}

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if(table.material[i].type >= 0)
			material[table.material[i].type] += 1;
	}
	for (int recipeNum = 0; recipeNum < recipeList_.size(); recipeNum++)
	{
		for (int materialNum = 0; materialNum < materialName_.size(); materialNum++)
		{
			if (material[materialNum] < recipeList_[recipeNum].ratio[materialNum])
			{
				break;
			}

			if (materialNum == materialName_.size() - 1)
			{
				result = recipeNum;
			}
		}
	}

	return result;
}

void MaterialTable::TableReset()
{
	//
	int makePuppetType = MakePuppet();
	//�L�����쐬�ł���������
	bool isMade = storage->AddStorage(makePuppetType);

	//�L�����쐬�ł����ꍇ�A���݂̃e�[�u����Ԃ�ۑ����� QuickRecipe �ɒǉ�
	if (isMade)
	{

		std::vector<int> currentRecipe(MATERIAL_TYPE::MATERIAL_END, 0);
		for (int i = 0; i < TABLE_SIZE; i++)
		{
			if (table.material[i].name == "frame")
				break;
			if (table.material[i].type >= 0 && table.material[i].type < MATERIAL_TYPE::MATERIAL_END)
			{
				currentRecipe[table.material[i].type]++;
			}
		}
		steamAnim_.Start();
		quickRecipe->AddRecipe(makePuppetType, currentRecipe);
	}

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (table.material[i].name == "frame")
			break;
		if (!isMade)
		{
			if (rand() % 100 <= returnProbability_)
				materialList_[table.material[i].type].num++;
		}

		table.material[i].type = MATERIAL_TYPE::EMPTY;
		table.material[i].name = "frame";
		table.material[i].button.ChangeImage("Image\\frame.png");
	}
	table.num = 0;
}

int MaterialTable::GetSelectStragePuppet()
{
	storage->puppetList_[storage->selectPuppetNumber].num--;
	return storage->selectPuppetNumber;
}

bool MaterialTable::isNotEmpty()
{
	return storage->puppetList_[storage->selectPuppetNumber].num > 0;
}

void MaterialTable::GetRandomMaterial()
{
	int randNum = rand() % materialName_.size();
	materialList_[randNum].num++;
	addAnim_.SetPosition({ (float)materialList_[randNum].buttonPosition.x,(float)materialList_[randNum].buttonPosition.y,0 });
	addAnim_.Start();
}

void MaterialTable::GiveMaterial(int _num)
{
	for (int i = 0; i < materialName_.size(); i++)
	{
		materialList_[i].num = _num;
	}
}

void MaterialTable::InitFrameTable(int _row, CsvReader _csv)
{
	hTable_ = Image::Load("Image\\" + _csv.GetString(_row, NAME) + ".png");
	assert(hTable_ >= 0);
	Transform ftrans;
	ftrans.position_ = { _csv.GetFloat(_row,POSITION_X),_csv.GetFloat(_row,POSITION_Y),1.0 };
	ftrans.scale_ = { _csv.GetFloat(_row,SCALE_X),_csv.GetFloat(_row,SCALE_Y),1.0 };
	Image::SetTransform(hTable_, ftrans);
}

void MaterialTable::InitMaterialList(int _row, CsvReader _csv)
{
	for (int i = 0; i < materialName_.size(); i++)
	{
		materialList_[i].type = i;

		materialList_[i].name = materialName_[i];

		std::string fileName = "Image\\" + materialList_[i].name + ".png";
		materialList_[i].button.LoadButtonImage(fileName);

		materialList_[i].buttonPosition.x = i % TABLE_SIZE * materialList_[i].button.GetSize().x + _csv.GetFloat(_row, POSITION_X);
		materialList_[i].buttonPosition.y = i / TABLE_SIZE * materialList_[i].button.GetSize().y + _csv.GetFloat(_row, POSITION_Y);
		materialList_[i].button.Initialize(materialList_[i].buttonPosition.x, materialList_[i].buttonPosition.y);
		materialList_[i].num = 0;
		materialList_[i].numText.Initialize();
		materialList_[i].textPosition.x = _csv.GetFloat(MATERIAL_NUM, POSITION_X);
		materialList_[i].textPosition.y = _csv.GetFloat(MATERIAL_NUM, POSITION_Y);
	}
}

void MaterialTable::InitMakeButton(int _row, CsvReader _csv)
{
	makeButton_.Initialize(_csv.GetFloat(_row, POSITION_X), _csv.GetFloat(_row, POSITION_Y), "Image\\" + _csv.GetString(_row, NAME) + ".png");
}

void MaterialTable::InitSlotFrames(int _row, CsvReader _csv)
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		table.material[i].type = MATERIAL_TYPE::EMPTY;
		table.material[i].buttonPosition.x = i * table.material[i].button.GetSize().x + _csv.GetFloat(_row, POSITION_X);
		table.material[i].buttonPosition.y = _csv.GetFloat(_row, POSITION_Y);
		table.material[i].button.Initialize(table.material[i].buttonPosition.x, table.material[i].buttonPosition.y, "Image\\frame.png");

		table.material[i].buttonPosition.x = i * table.material[i].button.GetSize().x + _csv.GetFloat(_row, POSITION_X);
		table.material[i].buttonPosition.y = _csv.GetFloat(_row, POSITION_Y);
		table.material[i].button.SetPosition({ (float)table.material[i].buttonPosition.x ,(float)table.material[i].buttonPosition.y,0 });

		table.material[i].name = "frame";
	}
}