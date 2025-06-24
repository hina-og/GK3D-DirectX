#include "Recipe.h"
#include "PuppetFactory.h"
#include "ImageDataUtil.h"

QuickRecipe::QuickRecipe(GameObject* parent)
	: GameObject(parent, "QuickRecipe")
{
	inData = false;
}

void QuickRecipe::Initialize()
{
	//CsvReader csv;
	//csv.Load("GameData\\IMageData.csv");
	//XMINT2 position = { csv.GetInt(14,1),csv.GetInt(14,2) };


	//saveRecipeBtn.LoadButtonImage("Image\\" + csv.GetString(14, 0) + ".png");
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

void QuickRecipe::SetPosition(int _row, CsvReader _csv)
{
	saveRecipeBtn.LoadButtonImage("Image\\" + _csv.GetString(_row, NAME) + ".png");
	saveRecipeBtn.Initialize(_csv.GetInt(_row, POSITION_X), _csv.GetInt(_row, POSITION_Y));
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

