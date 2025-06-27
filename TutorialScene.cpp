#include "TutorialScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Movie.h"

TutorialScene::TutorialScene(GameObject* parent)
{
}

void TutorialScene::Initialize()
{
	hMovie_ = Movie::Load("Movie\\TutorialMovie.wmv");
	assert(hMovie_ >= 0);
	Movie::Play(hMovie_);
}

void TutorialScene::Update()
{
	if (Input::IsMouseButtonDown(LEFT_CLICK) || Input::IsKeyDown(DIK_RETURN) || Movie::IsEnded(hMovie_))
	{
		Movie::Stop(hMovie_);
		Movie::Release(hMovie_);
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_TITLE);
	}
}

void TutorialScene::Draw()
{
}

void TutorialScene::Release()
{
}
