#include "GameOver.h"
#include "Engine/Direct3D.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"

GameOver::GameOver(GameObject* parent)
	: GameObject(parent, "GameOver")
{
}

void GameOver::Initialize()
{
	hBord_ = Image::Load("Image\\GameOver.png");
	assert(hBord_ >= 0);
	transform_.position_.y = Direct3D::screenHeight_ + Image::GetImageSize(hBord_).y;
}

void GameOver::Update()
{
	if (transform_.position_.y > Image::GetImageSize(hBord_).y / 2)
	{
		float downSpeed = 10.0;
		transform_.position_.y -= downSpeed;
	}
	else
	{
		if (Input::IsMouseButtonDown(LEFT_CLICK))
		{
			SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
			pSM->ChangeScene(SCENE_ID::SCENE_ID_TITLE);
		}
		transform_.position_.y = Image::GetImageSize(hBord_).y / 2;
	}
}

void GameOver::Draw()
{
	Image::SetPosition(hBord_, transform_.position_);
	Image::Draw(hBord_);
}

void GameOver::Release()
{
}
