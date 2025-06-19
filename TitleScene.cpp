#include "TitleScene.h"
#include "Engine/Image.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "SetUp.h"
#include "Engine/Movie.h"
#include "File.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hPict_(-1)
{
}

void TitleScene::Initialize()
{
	hPict_ = Image::Load("Image\\Title.png");

	selectStage_ = Instantiate<SelectStage>(this);
	selectStage_->SetStageNum(File::GetFileNum("GameData\\StageData"));

	pSM = (SceneManager*)(FindObject("SceneManager"));
}

void TitleScene::Update()
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

void TitleScene::Draw()
{
	Image::SetTransform(hPict_, transform_);
	Image::Draw(hPict_);
}

void TitleScene::Release()
{
	Image::AllRelease();
}