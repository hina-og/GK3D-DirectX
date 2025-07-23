#include "Recipe.h"
#include "PuppetFactory.h"
#include "ImageDataUtil.h"
#include <unordered_map>

namespace {
	//�摜���L���b�V��
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
		//�f�[�^�������Ă��Ȃ���Ԃɂ���
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
			//�{�^���̃x�[�X�̕`��
			qButton[i].saveRecipeBtn.SetPosition({ position_.x,position_.y + i * -blankInterval.y,0 });
			qButton[i].saveRecipeBtn.Draw();

			//�L�����̉摜�̕`��
			Image::SetPosition(qButton[i].charaIcon, { (position_.x - qButton[i].saveRecipeBtn.GetSize().x / 2 + Image::GetImageSize(qButton[i].charaIcon).x), position_.y + i * -blankInterval.y,0 });
			Image::Draw(qButton[i].charaIcon);

			//�f�މ摜�̕`��
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
	//�ۑ����郌�V�s�̍ő吔���Q�ȏ�̏ꍇ�͂P�����炷
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
	ftrans.x += (Image::GetImageSize(qButton[index].charaIcon).x / Direct3D::screenWidth_)//�L�����摜�̃T�C�Y���ʒu�Ɠ����P�ʂɕϊ��i�ʒu�͍�-1�A�E+1�j
						 - qButton[0].saveRecipeBtn.GetSize().x / 2 / Direct3D::screenWidth_;//�x�[�X�{�^���̍����ɃL�����摜�������Ă���
	
	qButton[index].recipeIcon.clear();
	for (int i = 0;i < MATERIAL_END;i++)
	{
		if (_recipe[i] >= 0)
		{
			//���V�s�̉摜�ǂݍ���
			std::string path = "Image\\" + GetMaterialNameFromType(i) + ".png";
			for (int j = 0;j < _recipe[i];j++)
			{
				qButton[index].recipeIcon.push_back(LoadImageCached(path));
			}
		}
	}

	qButton[index].inData = true;
}

