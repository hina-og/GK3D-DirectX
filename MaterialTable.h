#pragma once
#include "Engine/GameObject.h"
#include "Engine/CsvReader.h"
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

class MaterialTable
	: public GameObject
{
	enum IMAGE_ROW
	{
		TABLE = 1,
		MATERIAL,
		MATERIAL_NUM,
		MATERIAL_TABLE,
		MAKE,
		QUICK_RECIPE,
		POT,
	};

	int initMaterialNum;
	std::vector<std::string> materialName_;

	//画像ハンドル
	int hTable_;//
	int hPot_;//大釜

	//音ハンドル
	int hSelect_;
	int hChoise_;

	Material materialList_[MATERIAL_TYPE::MATERIAL_END];
	Table table;
	PuppetStorage* storage;

	Button makeButton_;
	Animation addAnim_;
	Animation steamAnim_;

	std::vector<Recipe> recipeList_;

	//作成失敗したときに素材が戻ってくる確率
	int returnProbability_;


	QuickRecipe* quickRecipe;

	std::vector<int> materialProbability_;
	int totalProbability_;

	void InitFrameTable(int _row, CsvReader _csv);
	void InitMaterialList(int _row, CsvReader _csv);
	void InitMakeButton(int _row, CsvReader _csv);
	void InitSlotFrames(int _row, CsvReader _csv);

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