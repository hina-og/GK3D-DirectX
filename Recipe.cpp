#include "Recipe.h"
#include "Engine/CsvReader.h"

QuickRecipe::QuickRecipe(GameObject* parent)
	: GameObject(parent, "QuickRecipe")
{
	inData = false;
}

void QuickRecipe::Initialize()
{
	CsvReader csv;
	csv.Load("GameData\\IMageData.csv");
	XMINT2 position = { csv.GetInt(14,1),csv.GetInt(14,2) };


	saveRecipeBtn.LoadButtonImage("Image\\" + csv.GetString(14, 0) + ".png");
}

void QuickRecipe::Update()
{
	if (inData)
	{
		saveRecipeBtn.Update();
	}
}

void QuickRecipe::Draw()
{
	if (inData)
	{
		saveRecipeBtn.Draw();
	}
}

void QuickRecipe::Release()
{
	saveRecipeBtn.Release();
}

void QuickRecipe::SetPosition(int _x, int _y)
{
	saveRecipeBtn.Initialize(_x,_y);
}

XMFLOAT3 QuickRecipe::GetSize()
{
	return saveRecipeBtn.GetSize();
}

void QuickRecipe::AddRecipe(int _charaType, std::vector<int> _recipe)
{
	charaIcon = _charaType;
	recipeIcon = _recipe;
	inData = true;
}

