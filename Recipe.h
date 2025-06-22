#pragma once
#include <vector>
#include "Button.h"
#include "Engine/Text.h"

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

class Recipe
{
public:
	std::vector<int> ratio;//ëfçﬁÇÃäÑçá
	int puppetType;//çÏÇÍÇÈÉLÉÉÉâ
};

const int saveNum{ 3 };

class QuickRecipe
	: public GameObject
{
	Button saveRecipeBtn;
	int charaIcon;
	std::vector<int>recipeIcon;
	std::vector<int>material;
	bool inData;
public:

	QuickRecipe(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();
	void SetPosition(int _x, int _y);
	XMFLOAT3 GetSize();
	void AddRecipe(int _charaType,std::vector<int> _recipe);
};
