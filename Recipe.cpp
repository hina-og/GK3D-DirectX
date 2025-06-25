#include "Recipe.h"
#include "PuppetFactory.h"
#include "ImageDataUtil.h"

QuickRecipe::QuickRecipe(GameObject* parent)
	: GameObject(parent, "QuickRecipe")
{
	for (int i = 0;i < saveNum;i++)
	{
		qButton[i].inData = false;
	}
}

void QuickRecipe::Initialize()
{
}

void QuickRecipe::Update()
{
	for (int i = 0;i < saveNum;i++)
	{
		if (qButton[i].inData)
		{
			qButton[i].saveRecipeBtn.Update();
		}
	}
}

void QuickRecipe::Draw()
{
	for (int i = 0;i < saveNum;i++)
	{
		if (qButton[i].inData)
		{
			XMFLOAT3 blankInterval = { 100.0f,128.0f,0.0f };

			qButton[i].saveRecipeBtn.SetPosition({ position_.x,position_.y + i * -blankInterval.y,0 });
			qButton[i].saveRecipeBtn.Draw();
			Image::SetPosition(qButton[i].charaIcon, { (position_.x - qButton[i].saveRecipeBtn.GetSize().x / 2 + Image::GetImageSize(qButton[i].charaIcon).x) / Direct3D::screenWidth_ , position_.y / Direct3D::screenHeight_ + i * -blankInterval.y / Direct3D::screenHeight_,0 });
			Image::Draw(qButton[i].charaIcon);
			for (int j = 0;j < qButton[i].recipeIcon.size();j++)
			{
				Image::SetPosition(qButton[i].recipeIcon[j], { (position_.x - Image::GetImageSize(qButton[i].recipeIcon[j]).x + j * blankInterval.x) / Direct3D::screenWidth_, (position_.y - i * blankInterval.y) / Direct3D::screenHeight_, 0});
				Image::Draw(qButton[i].recipeIcon[j]);
			}
		}
	}
}

void QuickRecipe::Release()
{
	for (int i = 0;i < saveNum;i++)
	{
		qButton[i].saveRecipeBtn.Release();
	}
}

void QuickRecipe::SetPosition(int _row, CsvReader _csv)
{
	position_ = { _csv.GetFloat(_row, POSITION_X), _csv.GetFloat(_row, POSITION_Y) ,0 };
	for (int i = 0;i < saveNum;i++)
	{
		qButton[i].saveRecipeBtn.LoadButtonImage("Image\\" + _csv.GetString(_row, NAME) + ".png");
		qButton[i].saveRecipeBtn.Initialize(position_.x,position_.y);
	}
}

XMFLOAT3 QuickRecipe::GetSize()
{
	return qButton[0].saveRecipeBtn.GetSize();
}

void QuickRecipe::AddRecipe(int _charaType, std::vector<int> _recipe)
{
	for (int i = saveNum - 2;i >= 0;i--)
	{
		if (qButton[i].inData)
		{
			qButton[i + 1] = qButton[i];
			qButton[i + 1].inData = true;
		}
	}
	int firstNum = 0;
	qButton[firstNum].charaIcon = Image::Load("Image\\" + GetCharacterNameFromType(_charaType) + ".png");
	XMFLOAT3 ftrans = qButton[firstNum].saveRecipeBtn.GetPosition();
	ftrans.x += Image::GetImageSize(qButton[firstNum].charaIcon).x / Direct3D::screenWidth_ - qButton[0].saveRecipeBtn.GetSize().x / 2 / Direct3D::screenWidth_;
	qButton[firstNum].recipeIcon.clear();
	for (int i = 0;i < MATERIAL_END;i++)
	{
		if (_recipe[i] >= 0)
		{
			for (int j = 0;j < _recipe[i];j++)
			{
				std::string str = "Image\\" + GetMaterialNameFromType(i) + ".png";
				qButton[firstNum].recipeIcon.push_back(Image::Load(str));
			}
		}
	}
	qButton[firstNum].inData = true;
}

