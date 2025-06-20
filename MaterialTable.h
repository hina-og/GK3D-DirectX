#pragma once
#include "Engine/GameObject.h"
#include "Button.h"
#include "Puppet.h"
#include <vector>
#include "PuppetStorage.h"
#include "Animation.h"
#include "Recipe.h"

const int TABLE_SIZE{ 4 };



struct Table
{
	Material material[TABLE_SIZE];
	int x;
	int y;

	int num;
};

enum IMAGE_DATA//ì«Ç›çûÇﬁâÊëúÉfÅ[É^ÇÃèáî‘
{
	name = 0,
	posX,
	posY,
	scaleX,
	scaleY,
};

class MaterialTable
	: public GameObject
{
	int initMaterialNum;
	std::vector<std::string> materialName_;

	int hTable_;

	//âπ
	int hSelect_;
	int hChoise_;

	Material materialList_[MATERIAL_TYPE::MATERIAL_END];
	Table table;
	PuppetStorage* storage;

	Button makeButton_;
	Animation addAnim_;

	std::vector<Recipe> recipeList_;

	int returnProbability_;

	MATERIAL_TYPE StringToMaterialType(const std::string& name);

	QuickRecipe* quickRecipe;
public:
	MaterialTable(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	void ReadRecipe();
	int MakePuppet();
	void TableReset();
	int GetSelectStragePuppet();
	bool isNotEmpty();
	void GetRandomMaterial();
	void GiveMaterial(int _num);
};