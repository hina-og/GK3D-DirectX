#pragma once
#include "Engine/GameObject.h"
#include "Engine/CsvReader.h"
#include "Button.h"
#include "Puppet.h"
#include <vector>
#include "PuppetStorage.h"
#include "Animation.h"
#include "Recipe.h"

//素材を選択できる最大数
const int TABLE_SIZE{ 4 };

//保持できる素材の最大数
const int MAX_MATERIAL_NUM{ 99 };

//選択した素材を並べるやつ
struct Table
{
	Material material[TABLE_SIZE];//素材を置く枠みたいなやつ
	int x, y;//表示位置
	int num;//並べられている素材の数
};

class MaterialTable
	: public GameObject
{
	

	//MaterialTableDataの行
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

	std::vector<std::string> materialName_;//素材の名前

	//画像ハンドル
	int hTable_;//後ろの木の板
	int hPot_;//大釜

	//音ハンドル
	const int selectSoundNum{ 3 };//hSelectを同時にならす最大数
	int hSelect_;//作成音
	int hChoise_;//選択音

	Material materialList_[MATERIAL_TYPE::MATERIAL_END];//素材の配列
	Table table;//選択した素材を並べるやつ
	PuppetStorage* storage;//キャラのストレージ

	Button makeButton_;//作るボタン
	Animation addAnim_;//素材が増えたときのアニメーション
	Animation steamAnim_;//作ったときのボワッてするアニメーション

	std::vector<Recipe> recipeList_;//レシピリスト

	int returnProbability_;//作成失敗したときに素材が戻ってくる確率

	QuickRecipe* quickRecipe;//作ったレシピの保存

	void InitFrameTable(int _row, CsvReader _csv);
	void InitMaterialList(int _row, CsvReader _csv);
	void InitMakeButton(int _row, CsvReader _csv);
	void InitSlotFrames(int _row, CsvReader _csv);

	bool isTutorial;

public:
	MaterialTable(GameObject* parent);
	void Initialize();
	void Update();
	void Draw();
	void Release();

	void TutorialInitialize();

	//レシピの読み込み
	void ReadRecipe();
	//キャラを作る
	int MakePuppet();
	//並べている素材をリセットする（作成に失敗したら一定確率で素材を戻す）
	void TableReset();
	//選択しているキャラの番号を返す+そのキャラの数を減らす
	int GetSelectStragePuppet();
	//選択しているキャラが1体以上いるか
	bool isNotEmpty();
	//ランダムで素材が配られる
	void GetRandomMaterial();
	//_numの数だけ最初に素材が与えられる
	void GiveMaterial(int _num);
};