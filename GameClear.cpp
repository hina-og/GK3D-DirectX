#include "GameClear.h"
#include "Engine/Direct3D.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"

GameClear::GameClear(GameObject* parent)
	: GameObject(parent, "GameClear")
{
}

void GameClear::Initialize()
{
	hBord_ = Image::Load("Image\\GameClear.png");
	assert(hBord_ >= 0);
	transform_.position_.y = 1 + (Image::GetImageSize(hBord_).y / 2 / Direct3D::screenHeight_);
}

void GameClear::Update()
{
	if (transform_.position_.y > 1 - (Image::GetImageSize(hBord_).y / Direct3D::screenHeight_))
	{
		float downSpeed = 15.0;
		transform_.position_.y -= downSpeed / Direct3D::screenHeight_;
	}
	else
	{
		if (Input::IsMouseButtonDown(LEFT_CLICK))
		{
			SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
			pSM->ChangeScene(SCENE_ID::SCENE_ID_TITLE);
		}
		transform_.position_.y = 1 - (Image::GetImageSize(hBord_).y / Direct3D::screenHeight_);
	}
}

void GameClear::Draw()
{
	Image::SetPosition(hBord_, transform_.position_);
	Image::Draw(hBord_);
}

void GameClear::Release()
{
}
