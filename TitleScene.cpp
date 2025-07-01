#include "TitleScene.h"
#include "Engine/Image.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/Movie.h"
#include "SetUp.h"
#include "File.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hBackGround_(-1)
{
}

void TitleScene::Initialize()
{
	//”wŒi
	hBackGround_ = Image::Load("Image\\TitleBackGround.png");
	assert(hBackGround_ >= 0);

	//‘€ìà–¾
	hOperation_ = Image::Load("Image\\OperationButton.png");
	assert(hOperation_ >= 0);

	hLogo_ = Image::Load("Image\\TitleLogo.png");
	assert(hLogo_ >= 0);

	canSelect_ = false;

	pSM = (SceneManager*)(FindObject("SceneManager"));

	buttonText_ = new Text;
	buttonText_->Initialize();
}

void TitleScene::Update()
{
	if (canSelect_)
	{
		if (selectStage_->decision_)
		{
			if (selectStage_->GetStageNum() == 0)
			{
				pSM->ChangeScene(SCENE_ID::SCENE_ID_TUTORIAL);
			}
			else
			{
				SetUp::currentDifficulty = File::GetFileName("GameData\\StageData", selectStage_->GetStageNum());
				pSM->ChangeScene(SCENE_ID::SCENE_ID_PLAY);
			}
		}
	}
	else
	{
		if (Input::IsKeyDown(DIK_SPACE))
		{
			selectStage_ = Instantiate<SelectStage>(this);
			selectStage_->SetStageNum(File::GetFileNum("GameData\\StageData"));
			canSelect_ = true;
		}
	}
}

void TitleScene::Draw()
{
	Image::SetTransform(hBackGround_, transform_);
	Image::Draw(hBackGround_);

	if (canSelect_)
	{
		XMFLOAT3 tampPos = { 0,-1 + Image::GetImageSize(hOperation_).y / Direct3D::screenHeight_,0 };
		Image::SetPosition(hOperation_, tampPos);
		Image::Draw(hOperation_);
	}
	else
	{
		Image::Draw(hLogo_);
		buttonText_->Draw(Direct3D::screenWidth_ / 2, Direct3D::screenHeight_, "SPACE");
	}
}

void TitleScene::Release()
{
	Image::AllRelease();
}