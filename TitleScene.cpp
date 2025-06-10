#include "TitleScene.h"
#include "Engine/Image.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "SetUp.h"
#include "Engine/Movie.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hPict_(-1)
{
}

void TitleScene::Initialize()
{
	hPict_ = Image::Load("Image\\Title.png");

	canStart_ = false;
	selectStage_ = Instantiate<SelectStage>(this);

	pSM = (SceneManager*)(FindObject("SceneManager"));
}

void TitleScene::Update()
{
	if (canStart_)
	{
		pSM->ChangeScene(SCENE_ID::SCENE_ID_PLAY);
	}

	if (Input::IsKey(DIK_D) && Input::IsKey(DIK_I) && Input::IsKey(DIK_E))
	{
		SetUp::currentDifficulty = Difficulty::Despair;
		canStart_ = true;
	}

	if (selectStage_->decision_)
	{
		switch (selectStage_->GetStageNum())
		{
		case 0:
			SetUp::currentDifficulty = Difficulty::Easy;
			break;
		case 1:
			SetUp::currentDifficulty = Difficulty::Normal;
			break;
		case 2:
			SetUp::currentDifficulty = Difficulty::Hard;
			break;
		case 3:
			pSM->ChangeScene(SCENE_ID::SCENE_ID_TUTORIAL);
			break;
		default:
			break;
		}
		canStart_ = true;
	}


}

void TitleScene::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

void TitleScene::Release()
{
	Image::AllRelease();
}