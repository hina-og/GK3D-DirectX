#include "TitleScene.h"
#include "Engine/Image.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "SetUp.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hPict_(-1)
{
}

void TitleScene::Initialize()
{
	hPict_ = Image::Load("Image\\Title.png");

	easy_.Initialize(0, Direct3D::screenHeight_ / 4 * 1, "Image\\Easy.png");
	normal_.Initialize(0, Direct3D::screenHeight_ / 4 * 2, "Image\\Normal.png");
	hard_.Initialize(0, Direct3D::screenHeight_ / 4 * 3, "Image\\Hard.png");

	canStart_ = false;
}

void TitleScene::Update()
{
	easy_.Update();
	normal_.Update();
	hard_.Update();

	if (Input::IsKey(DIK_D) && Input::IsKey(DIK_I) && Input::IsKey(DIK_E))
	{
		SetUp::currentDifficulty = Difficulty::Despair;
		canStart_ = true;
	}

	if (easy_.isPress_)
	{
		SetUp::currentDifficulty = Difficulty::Easy;
		canStart_ = true;
	}
	if (normal_.isPress_)
	{
		SetUp::currentDifficulty = Difficulty::Normal;
		canStart_ = true;
	}
	if (hard_.isPress_)
	{
		SetUp::currentDifficulty = Difficulty::Hard;
		canStart_ = true;
	}

	if (canStart_)
	{
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_PLAY);
	}
}

void TitleScene::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);

	easy_.Draw();
	normal_.Draw();
	hard_.Draw();
}

void TitleScene::Release()
{
	Image::AllRelease();
}
