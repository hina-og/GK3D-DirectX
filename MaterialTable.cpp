#include "MaterialTable.h"
#include "Engine/CsvReader.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"
#include "Engine/Direct3D.h"

MaterialTable::MaterialTable(GameObject* parent)
	: GameObject(parent, "MaterialTable"), hSelect_(-1)
{
}

void MaterialTable::Initialize()
{

	CsvReader csv("GameData\\ImageData.csv");


	storage = Instantiate<PuppetStorage>(this);
	storage->LoadImageData(csv);



	CsvReader csvMaterial("GameData\\Recipe.csv");
	materialName_.resize(csvMaterial.GetColumns(0) - 1);
	for (int column = 1; column < csvMaterial.GetColumns(0); column++)
	{
		materialName_[column - 1] += csvMaterial.GetString(0, column);
	}


	hTable_ = Image::Load("Image\\" + csv.GetString(1, name) + ".png");
	assert(hTable_ >= 0);
	Transform ftrans;
	ftrans.position_ = { csv.GetFloat(1,posX),csv.GetFloat(1,posY),1.0 };
	ftrans.scale_ = { csv.GetFloat(1,scaleX),csv.GetFloat(1,scaleY),1.0 };
	Image::SetTransform(hTable_, ftrans);
	

	hTableFrame_ = Image::Load("Image\\" + csv.GetString(5, name) + ".png");
	assert(hTableFrame_ >= 0);
	Image::SetTransform(hTableFrame_, ftrans);


	for (int i = 0; i < materialName_.size(); i++)
	{
		materialList_[i].type = i;

		materialList_[i].name = materialName_[i];

		std::string fileName = "Image\\" + materialList_[i].name + ".png";
		materialList_[i].button.LoadButtonImage(fileName);

		materialList_[i].x = i % TABLE_SIZE * materialList_[i].button.GetSize().x + csv.GetFloat(2, posX);
		materialList_[i].y = i / TABLE_SIZE * materialList_[i].button.GetSize().y + csv.GetFloat(2, posY);
		materialList_[i].button.Initialize(materialList_[i].x, materialList_[i].y);
		materialList_[i].num = INIT_MATERIAL_NUM;
		materialList_[i].text.Initialize();
		materialList_[i].textX = csv.GetFloat(6, posX);
		materialList_[i].textY = csv.GetFloat(6, posY);
	}


	for (int i = 0; i < TABLE_SIZE; i++)
	{
		table.material[i].type = MATERIAL_TYPE::EMPTY;
		table.material[i].x = i * table.material[i].button.GetSize().x + csv.GetFloat(3, posX);
		table.material[i].y = csv.GetFloat(3, posY);
		table.material[i].button.Initialize(table.material[i].x, table.material[i].y, "Image\\empty.png");

		table.material[i].x = i * table.material[i].button.GetSize().x + csv.GetFloat(3,posX);
		table.material[i].y = csv.GetFloat(3,posY);
		table.material[i].button.SetPosition({ (float)table.material[i].x ,(float)table.material[i].y,0 });

		table.material[i].name = "empty";
	}


	makeButton_.Initialize(csv.GetFloat(4,posX), csv.GetFloat(4,posY), "Image\\" + csv.GetString(4,name) + ".png");

	addAnim_.Initialize("Image\\flashAnim.png", 0, 0, 64, 64, false, 3, false);
	addAnim_.SetSpeed(0.15);

	//hSelect_ = Audio::Load("Sounds\\SE\\add.wav");


	ReadRecipe();
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


	//素材を選択したらテーブルに置く
	for (int i = 0; i < materialName_.size(); i++)
	{
		if (materialList_[i].button.isPress_ && table.num < TABLE_SIZE && 0 < materialList_[i].num)
		{
			table.material[table.num].type = materialList_[i].type;
			table.material[table.num].name = materialList_[i].name;
			table.material[table.num].button.ChangeImage("Image\\" + table.material[table.num].name + ".png");
			table.num++;
			materialList_[i].num--;
		}
	}


	//選択したテーブルの素材を戻す
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (table.material[i].button.isPress_ && 0 < table.num && table.material[i].name != "empty")
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
			table.material[table.num].name = "empty";
			table.material[table.num].button.ChangeImage("Image\\empty.png");

		}
	}
	if (makeButton_.isPress_)
	{
		storage->AddStorage(MakePuppet());
		TableReset();
		//Audio::Play(hSelect_);
	}
	addAnim_.Update();
}

void MaterialTable::Draw()
{
	Image::Draw(hTable_);

	for (int i = 0; i < materialName_.size(); i++)
	{
		materialList_[i].button.Draw();
		materialList_[i].text.Draw(materialList_[i].x + materialList_[i].textX, materialList_[i].y + materialList_[i].textY, materialList_[i].num);
	}

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		table.material[i].button.Draw();
	}
	
	Image::Draw(hTableFrame_);

	makeButton_.Draw();
	addAnim_.Draw();
}

void MaterialTable::Release()
{
}

void MaterialTable::ReadRecipe()
{
	CsvReader csv("GameData\\Recipe.csv");
	
	for (int line = 1;line < csv.GetLines();line++)
	{
		Recipe r;
		for (int column = 1;column < csv.GetColumns(line);column++)
		{
			r.ratio[column - 1] = csv.GetInt(line, column);
		}
		r.puppetType = line - 1;
		recipeList_.push_back(r);
	}
}

int MaterialTable::MakePuppet()
{
	int resoult = MOUSE;
	int material[MATERIAL_END];
	for (int i = 0; i < materialName_.size(); i++)
	{
		material[i] = 0;
	}

	for (int i = 0; i < TABLE_SIZE; i++)
	{
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
				resoult = recipeNum;
			}
		}
	}

	return resoult;
}

void MaterialTable::TableReset()
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		table.material[i].type = MATERIAL_TYPE::EMPTY;
		table.material[i].name = "empty";
		table.material[i].button.ChangeImage("Image\\empty.png");
		table.num = 0;
	}
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
	addAnim_.SetPosition({ (float)materialList_[randNum].x,(float)materialList_[randNum].y,0 });
	addAnim_.Start();
}

MATERIAL_TYPE MaterialTable::StringToMaterialType(const std::string& name)
{
	if (name == "BONE") 
		return BONE;
	if (name == "MEAT") 
		return MEAT;
	if (name == "SOUL") 
		return SOUL;
	if (name == "BACTERIA") 
		return BACTERIA;
	if (name == "ROCK") 
		return ROCK;
	if (name == "BRAIN") 
		return BRAIN;
	if (name == "WATER")
		return WATER;
	if (name == "EMPTY") 
		return EMPTY;

	return EMPTY;
}

CHARA_TYPE MaterialTable::StringToCharaType(const std::string& name)
{
	if (name == "MOUSE") 
		return MOUSE;
	if (name == "ZOMBIE") 
		return ZOMBIE;
	if (name == "MUSHROOM")
		return MUSHROOM;
	if (name == "SLIME")
		return SLIME;
	if (name == "GOLEM")
		return GOLEM;
	if (name == "GHOST")
		return GHOST;

	return MOUSE;
}
