#include "Recipe.h"
#include "PuppetFactory.h"
#include "ImageDataUtil.h"
#include <unordered_map>

namespace {
	//画像をキャッシュ
	std::unordered_map<std::string, int> g_imageCache;

	int LoadImageCached(const std::string& path) {
		auto it = g_imageCache.find(path);
		if (it != g_imageCache.end()) {
			return it->second;
		}
		int handle = Image::Load(path);
		g_imageCache[path] = handle;
		return handle;
	}

	XMFLOAT3 GetButtonPosition(const XMFLOAT3& basePos, int index, float spacingY = 128.0f) {
		return { basePos.x, basePos.y - index * spacingY, 0.0f };
	}
}

QuickRecipe::QuickRecipe(GameObject* parent)
	: GameObject(parent, "QuickRecipe")
{
}

void QuickRecipe::Initialize()
{
	for (int i = 0;i < MaxSaveNum;i++)
	{
		//データが入っていない状態にする
		qButton[i].inData = false;
	}
}

void QuickRecipe::Update()
{
	for (int i = 0;i < MaxSaveNum;i++)
	{
		if (qButton[i].inData)
		{
			qButton[i].saveRecipeBtn.Update();
		}
	}
}

void QuickRecipe::Draw()
{
	for (int i = 0;i < MaxSaveNum;i++)
	{
		if (qButton[i].inData)
		{
			//ボタンのベースの描画
			qButton[i].saveRecipeBtn.SetPosition({ position_.x,position_.y + i * -blankInterval.y,0 });
			qButton[i].saveRecipeBtn.Draw();

			//キャラの画像の描画
			Image::SetPosition(qButton[i].charaIcon, { (position_.x - qButton[i].saveRecipeBtn.GetSize().x / 2 + Image::GetImageSize(qButton[i].charaIcon).x), position_.y + i * -blankInterval.y,0 });
			Image::Draw(qButton[i].charaIcon);

			//素材画像の描画
			for (int j = 0;j < qButton[i].recipeIcon.size();j++)
			{
				Image::SetPosition(
					qButton[i].recipeIcon[j],
					{ 
						(position_.x - Image::GetImageSize(qButton[i].recipeIcon[j]).x + j * blankInterval.x),
						(position_.y - i * blankInterval.y),
						0
					}
				);
				Image::Draw(qButton[i].recipeIcon[j]);
			}
		}
	}
}

void QuickRecipe::Release()
{
	for (int i = 0;i < MaxSaveNum;i++)
	{
		qButton[i].Clear();
	}
}

void QuickRecipe::SetPosition(int _row, CsvReader _csv)
{
	position_ = { _csv.GetFloat(_row, POSITION_X), _csv.GetFloat(_row, POSITION_Y) ,0 };
	for (int i = 0;i < MaxSaveNum;i++)
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
	//保存するレシピの最大数が２つ以上の場合は１ずつずらす
	for (int i = MaxSaveNum - 2; i >= 0; --i)
	{
		if (qButton[i].inData)
		{
			qButton[i + 1] = std::move(qButton[i]);
		}
	}


	int index = 0;
	qButton[index].Clear();

	std::string charaPath = "Image\\" + GetCharacterNameFromType(_charaType) + ".png";
	qButton[index].charaIcon = LoadImageCached(charaPath);

	XMFLOAT3 ftrans = qButton[index].saveRecipeBtn.GetPosition();
	ftrans.x += (Image::GetImageSize(qButton[index].charaIcon).x / Direct3D::screenWidth_)//キャラ画像のサイズを位置と同じ単位に変換（位置は左-1、右+1）
						 - qButton[0].saveRecipeBtn.GetSize().x / 2 / Direct3D::screenWidth_;//ベースボタンの左側にキャラ画像を持ってくる
	
	qButton[index].recipeIcon.clear();
	for (int i = 0;i < MATERIAL_END;i++)
	{
		if (_recipe[i] >= 0)
		{
			//レシピの画像読み込み
			std::string path = "Image\\" + GetMaterialNameFromType(i) + ".png";
			for (int j = 0;j < _recipe[i];j++)
			{
				qButton[index].recipeIcon.push_back(LoadImageCached(path));
			}
		}
	}

	qButton[index].inData = true;
}

