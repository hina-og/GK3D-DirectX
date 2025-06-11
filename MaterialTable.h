#pragma once
#include "Engine/GameObject.h"
#include "Button.h"
#include "Puppet.h"
#include <vector>
#include "PuppetStorage.h"
#include "Animation.h"

const int TABLE_SIZE{ 4 };

enum MATERIAL_TYPE
{
	BONE = 0,
	MEAT,
	SOUL,
	BACTERIA,
	ROCK,
	BRAIN,
	WATER,
	MATERIAL_END,
	EMPTY,
};

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
	int num;
	Text text;
	int textX;
	int textY;
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
	int ratio[MATERIAL_END];//�f�ނ̊���
	int puppetType;//����L����
};

enum IMAGE_DATA//�ǂݍ��މ摜�f�[�^�̏���
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

	//��
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