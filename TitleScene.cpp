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
	hPict_ = Image::Load("Image\\TitleScene.png");

	easy_.Initialize(0, Direct3D::screenHeight_ / 4 * 1, "Image\\Easy.png");
	normal_.Initialize(0, Direct3D::screenHeight_ / 4 * 2, "Image\\Normal.png");
	hard_.Initialize(0, Direct3D::screenHeight_ / 4 * 3, "Image\\Hard.png");
}

void TitleScene::Update()
{
	easy_.Update();
	normal_.Update();
	hard_.Update();

	if (Input::IsKeyDown(DIK_RETURN))
	{
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_PLAY);
	}

	if (easy_.isPress_)
	{
		SetUp::currentDifficulty = Difficulty::Easy;
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_PLAY);
	}
	if (normal_.isPress_)
	{
		SetUp::currentDifficulty = Difficulty::Normal;
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_PLAY);
	}
	if (hard_.isPress_)
	{
		SetUp::currentDifficulty = Difficulty::Hard;
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
}
