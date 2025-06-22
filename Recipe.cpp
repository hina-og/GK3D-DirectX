#include "Recipe.h"
#include "Engine/CsvReader.h"
#include "PuppetFactory.h"

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
		Image::Draw(charaIcon);
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

	charaIcon = Image::Load("Image\\" + GetCharacterNameFromType(_charaType) + ".png");
	XMFLOAT3 ftrans = saveRecipeBtn.GetPosition();
	ftrans.x += Image::GetImageSize(charaIcon).x / Direct3D::screenWidth_ - saveRecipeBtn.GetSize().x / 2 / Direct3D::screenWidth_;
	Image::SetPosition(charaIcon, ftrans);
	recipeIcon = _recipe;
	inData = true;
}

