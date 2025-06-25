#pragma once
#include <vector>
#include "Button.h"
#include "Engine/Text.h"
#include "Engine/CsvReader.h"
#include <unordered_map>

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

inline int GetMaterialTypeFromName(const std::string& name)
{
	static const std::unordered_map<std::string, int> map = {
		{"Bone", MATERIAL_TYPE::BONE},
		{"Meat", MATERIAL_TYPE::MEAT},
		{"Soul", MATERIAL_TYPE::SOUL},
		{"Bacteria", MATERIAL_TYPE::BACTERIA},
		{"Rock", MATERIAL_TYPE::ROCK},
		{"Brain", MATERIAL_TYPE::BRAIN},
		{"Water", MATERIAL_TYPE::WATER}
	};
	auto it = map.find(name);
	return (it != map.end()) ? it->second : -1;
}

inline std::string GetMaterialNameFromType(int type)
{
	static const std::string names[] = {
		"Bone",
		"Meat",
		"Soul",
		"Bacteria",
		"Rock",
		"Brain",
		"Water"
	};
	if (type >= 0 && type < MATERIAL_TYPE::MATERIAL_END)
		return names[type];
	return "Unknown";
}

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
	struct QuickRecipeButton
	{
		Button saveRecipeBtn;
		int charaIcon;
		std::vector<int>recipeIcon;
		std::vector<int>material;
		bool inData;
	};

	QuickRecipeButton qButton[saveNum];

	XMFLOAT3 position_;
public:

	QuickRecipe(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();
	void SetPosition(int _row, CsvReader _csv);
	XMFLOAT3 GetSize();
	void AddRecipe(int _charaType,std::vector<int> _recipe);
};
