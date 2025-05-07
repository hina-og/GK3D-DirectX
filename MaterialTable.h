#pragma once
#include "Engine/GameObject.h"
#include "Button.h"
#include "Puppet.h"
#include <vector>
#include "PuppetStorage.h"

const int TABLE_SIZE{ 5 };
const int RECIPE_DATA_SIZE{ 6 };


enum MATERIAL_TYPE
{
	BONE = 0,
	MEAT,
	SOUL,
	BACTERIA,
	ROCK,
	BRAIN,
	EMPTY,
	FREE,
	MATERIAL_END,
};

const int MATERIAL_NUM{ MATERIAL_END - 2 };

enum MATERIAL_TAG
{
	
};

struct Material
{
	int type;
	std::vector<int> tag_;
	int x;
	int y;
	Button button;
	std::string name;
};

struct Table
{
	Material material[TABLE_SIZE];
	int x;
	int y;

	int num;
};

struct Recipe
{
	int materialType[TABLE_SIZE];
	int puppetType;
};

class MaterialTable
	: public GameObject
{
	int hTable_;
	Material materialList_[MATERIAL_TYPE::MATERIAL_END];
	Table table;
	PuppetStorage* storage;

	Button makeButton_;

	std::vector<Recipe> recipeList_;

	MATERIAL_TYPE StringToMaterialType(const std::string& name);
	CHARA_TYPE StringToCharaType(const std::string& name);
public:
	MaterialTable(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	void ReadRecipe();
	int MakePuppet();
	void TableReset();
};