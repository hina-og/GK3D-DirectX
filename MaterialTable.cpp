#include "MaterialTable.h"
#include "Engine/CsvReader.h"
#include "Engine/Image.h"
#include "Engine/Direct3D.h"

MaterialTable::MaterialTable(GameObject* parent)
	: GameObject(parent, "MaterialTable")
{
}

void MaterialTable::Initialize()
{
	storage = Instantiate<PuppetStorage>(this);

	hTable_ = Image::Load("Image\\MaterialTable.png");
	assert(hTable_ >= 0);

	transform_.scale_ = { 0.8,1.0,1.0 };
	Image::SeScale(hTable_, transform_.scale_);
	transform_.position_ = { -(float)Direct3D::screenWidth_ + Image::GetImageSize(hTable_).x,(float)Direct3D::screenHeight_ - Image::GetImageSize(hTable_).y,0 };
	Image::SetTransform(hTable_, transform_);

	std::string materialName_[MATERIAL_NUM]
	{
		"bone",
		"meat",
		"soul",
		"bacteria",
		"rock",
		"brain"
	};

	for (int i = 0; i < MATERIAL_NUM; i++)
	{
		materialList_[i].type = i;

		materialList_[i].name = materialName_[i];

		std::string fileName_ = "Image\\" + materialList_[i].name + ".png";
		materialList_[i].button.Initialize(transform_.position_.x + materialList_[i].x, transform_.position_.y + materialList_[i].y, fileName_);

		materialList_[i].x = transform_.position_.x + (i % TABLE_SIZE * materialList_[i].button.GetSize().x - Image::GetImageSize(hTable_).x + materialList_[i].button.GetSize().x / 2);
		materialList_[i].y = transform_.position_.y - (i / TABLE_SIZE * materialList_[i].button.GetSize().y - Image::GetImageSize(hTable_).y + materialList_[i].button.GetSize().y / 2);

		materialList_[i].button.SetPosition({ (float)materialList_[i].x ,(float)materialList_[i].y,0 });
	}

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		

		table.material[i].type = MATERIAL_TYPE::EMPTY;
		table.material[i].button.Initialize(table.material[i].x, table.material[i].y, "Image\\empty.png");


		table.material[i].x = i * table.material[i].button.GetSize().x - (Direct3D::screenWidth_ - table.material[i].button.GetSize().x / 2);
		table.material[i].y = table.y;
		table.material[i].button.SetPosition({ (float)table.material[i].x ,(float)table.material[i].y,0 });

		table.material[i].name = "empty";
	}

	makeButton_.Initialize(500,100, "Image\\Make.png");

	ReadRecipe();
}

void MaterialTable::Update()
{
	for (int i = 0; i < MATERIAL_NUM; i++)
	{
		materialList_[i].button.Update();
	}
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		table.material[i].button.Update();
	}
	makeButton_.Update();


	for (int i = 0; i < MATERIAL_NUM; i++)
	{
		if (materialList_[i].button.isPress_ && table.num < TABLE_SIZE)
		{
			table.material[table.num].type = materialList_[i].type;
			table.material[table.num].name = materialList_[i].name;
			table.material[table.num].button.Initialize(table.material[table.num].x, table.material[table.num].y, "Image\\" + table.material[table.num].name + ".png");
			table.num++;
		}
	}

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		if (table.material[i].button.isPress_ && 0 < table.num && table.material[i].name != "empty")
		{
			for (int j = i; j < TABLE_SIZE - 1; j++)
			{
				table.material[j].type = table.material[j + 1].type;
				table.material[j].name = table.material[j + 1].name;
				table.material[j].button.Initialize(table.material[j].x, table.material[j].y, "Image\\" + table.material[j].name + ".png");
			}
			table.num--;

			table.material[table.num].type = MATERIAL_TYPE::EMPTY;
			table.material[table.num].name = "empty";
			table.material[table.num].button.Initialize(table.material[table.num].x, table.material[table.num].y, "Image\\empty.png");
		}
	}

	if (makeButton_.isPress_)
	{
		storage->AddStorage(MakePuppet());
		TableReset();
	}
}

void MaterialTable::Draw()
{
	Image::Draw(hTable_);

	for (int i = 0; i < MATERIAL_NUM; i++)
	{
		materialList_[i].button.Draw();
	}

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		table.material[i].button.Draw();
	}
	makeButton_.Draw();
}

void MaterialTable::Release()
{
}

void MaterialTable::ReadRecipe()
{
	CsvReader csv("Assets\\GameData\\Recipe.csv");
	
	for (int line = 0;line < csv.GetLines();line++)
	{
		Recipe r;
		std::string recipeData[RECIPE_DATA_SIZE];
		for (int column = 0;column < csv.GetColumns(line);column++)
		{
			recipeData[column] = csv.GetString(line, column);
		}

		for (int i = 0; i < RECIPE_DATA_SIZE - 1; i++)
		{
			switch (StringToMaterialType(recipeData[i]))
			{
			case BONE:
				r.materialType[i] = MATERIAL_TYPE::BONE;
				break;
			case MEAT:
				r.materialType[i] = MATERIAL_TYPE::MEAT;
				break;
			case SOUL:
				r.materialType[i] = MATERIAL_TYPE::SOUL;
				break;
			case BACTERIA:
				r.materialType[i] = MATERIAL_TYPE::BACTERIA;
				break;
			case ROCK:
				r.materialType[i] = MATERIAL_TYPE::ROCK;
				break;
			case BRAIN:
				r.materialType[i] = MATERIAL_TYPE::BRAIN;
				break;
			case EMPTY:
				r.materialType[i] = MATERIAL_TYPE::EMPTY;
				break;
			default:
				r.materialType[i] = MATERIAL_TYPE::FREE;
				break;
			}
		}
		switch (StringToCharaType(recipeData[RECIPE_DATA_SIZE - 1]))
		{
		case MOUSE:
			r.puppetType = CHARA_TYPE::MOUSE;
			break;
		case ZOMBIE:
			r.puppetType = CHARA_TYPE::ZOMBIE;
			break;
		default:
			r.puppetType = CHARA_TYPE::FAILURE;
			break;
		}
		recipeList_.push_back(r);
	}
}

int MaterialTable::MakePuppet()
{
	int returnType = FAILURE;
	bool allTrue = false;

	for (int i = 0; i < TABLE_SIZE; i++)
	{
		for (int recipeNum = 0;recipeNum < recipeList_.size();recipeNum++)
		{
			for (int j = 0;j < TABLE_SIZE;j++)
			{
				if (recipeList_[recipeNum].materialType[j] != table.material[j].type)
				{
					break;
				}
				if (j == TABLE_SIZE - 1)
				{
					allTrue = true;
					returnType = recipeList_[recipeNum].puppetType;
				}
			}
		}
	}

	return returnType;
}

void MaterialTable::TableReset()
{
	for (int i = 0; i < TABLE_SIZE; i++)
	{
		table.material[i].type = MATERIAL_TYPE::EMPTY;
		table.material[i].name = "empty";
		table.material[i].button.Initialize(table.material[i].x, table.material[i].y, "Image\\empty.png");	
		
		table.num = 0;
	}
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
	if (name == "EMPTY") 
		return EMPTY;
	if (name == "FREE") 
		return FREE;

	return FREE;  // •s–¾‚Èê‡
}

CHARA_TYPE MaterialTable::StringToCharaType(const std::string& name)
{
	if (name == "MOUSE") 
		return MOUSE;
	if (name == "ZOMBIE") 
		return ZOMBIE;
	if (name == "FAILURE") 
		return FAILURE;

	return FAILURE;  // •s–¾‚Èê‡
}
