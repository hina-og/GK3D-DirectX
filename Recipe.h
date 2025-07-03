#pragma once
#include <vector>
#include "Button.h"
#include "Engine/Text.h"
#include "Engine/CsvReader.h"
#include <map>

//素材の種類
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
	EMPTY = -1,
};

//素材の名前→番号出力
inline int GetMaterialTypeFromName(const std::string& name)
{
	static const std::map<std::string, int> map = {
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

//素材の名前→番号出力（画像を読み込むときとか使う）
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
	int type;//種類
	//std::vector<int> tag_;//素材の相性とか決めたい
	XMFLOAT2 buttonPosition;//ボタンの位置(x,y)
	Button button;//素材ボタン
	std::string name;//名前
	int num;//持っている数
	Text numText;//持っている数のテキスト表示
	XMFLOAT2 textPosition;
};

class Recipe
{
public:
	std::vector<int> ratio;//素材の割合
	int puppetType;//作れるキャラ
};

//保存するレシピの数
const int saveNum{ 1 };

//クイックレシピ（ごちゃごちゃしそうだから消すかも）
class QuickRecipe
	: public GameObject
{
	struct QuickRecipeButton
	{
		Button saveRecipeBtn;//ベースのボタン
		int charaIcon;//キャラ画像
		std::vector<int>recipeIcon;//素材画像
		std::vector<int>material;//素材
		bool inData;//データが入っているか
	};

	QuickRecipeButton qButton[saveNum];

	XMFLOAT3 position_;
public:

	QuickRecipe(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();
	//csvで位置と画像を読み込む（_row：行）
	void SetPosition(int _row, CsvReader _csv);
	//ボタンサイズ
	XMFLOAT3 GetSize();
	/// <summary>
	/// クイックレシピ追加（数がsaveNumを超えると最初に入れたものから消えていく）
	/// </summary>
	/// <param name="_charaType">作れるキャラの種類</param>
	/// <param name="_recipe">選択した素材の配列</param>
	void AddRecipe(int _charaType,std::vector<int> _recipe);
};
