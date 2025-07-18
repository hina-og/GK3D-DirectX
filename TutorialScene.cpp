#include "TutorialScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Movie.h"

TutorialScene::TutorialScene(GameObject* parent)
{
}

void TutorialScene::Initialize()
{
	hBGM_ = Audio::Load("Sounds\\BGM\\BGM.wav");
	assert(hBGM_ >= 0);

	Audio::Play(hBGM_);

	stage = Instantiate<Stage>(this);
	player = Instantiate<Player>(this);
	enemy = Instantiate<Enemy>(this);
	material = Instantiate<MaterialTable>(this);
	material->TutorialInitialize();
	hud = Instantiate<HUD>(this);
	hud->TutorialInitialize();



	isReady = false;
	isBattleEnd = false;

	selectDir = Puppet::UP;
}

void TutorialScene::Update()
{
	//終了処理
	//if (Input::IsMouseButtonDown(LEFT_CLICK) || Input::IsKeyDown(DIK_RETURN) || Movie::IsEnded(hMovie_))
	//{
	//	Movie::Stop(hMovie_);
	//	Movie::Release(hMovie_);
	//	SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
	//	pSM->ChangeScene(SCENE_ID::SCENE_ID_TITLE);
	//}
	

	if (!Audio::IsPlaying(hBGM_))
	{
		Audio::Play(hBGM_);
	}

	int mouseX;
	int mouseY;

	Input::GetMousePosition(mouseX, mouseY);

	XMFLOAT2 mousePos = { (float)mouseX, (float)mouseY };
	stage->SelectTilePosition(mousePos);
	
	//ズームしていない場合にキャラの向きを変えられる
	if (!stage->IsZooming())
	{
		if (Input::IsKeyDown(DIK_S))
		{
			selectDir = Puppet::DOWN;
		}
		else if (Input::IsKeyDown(DIK_A))
		{
			selectDir = Puppet::LEFT;
		}
		else if (Input::IsKeyDown(DIK_W))
		{
			selectDir = Puppet::UP;
		}
		else if (Input::IsKeyDown(DIK_D))
		{
			selectDir = Puppet::RIGHT;
		}
	}

	hud->SetDirection(selectDir);

	//左クリックでキャラ配置
	if (Input::IsMouseButtonDown(LEFT_CLICK))
	{
		XMFLOAT2 tileNum = { -1, -1 };
		XMFLOAT3 tilePos = {};

		bool hit = stage->SelectTile(mousePos, tileNum, tilePos);
		if (hit && !stage->HasPlayer(tileNum) && material->isNotEmpty())
		{
			selectPos_ = tilePos;
			stage->PlaceCharacter(tileNum);
			player->unit_->AddCharacter(selectPos_, material->GetSelectStragePuppet(), selectDir);
		}
	}
	enemy->unit_->InvaderMove();



	player->unit_->InRange(enemy->unit_->GetPuppetArray());
	enemy->unit_->InRange(player->unit_->GetPuppetArray());

	if (isTimeStart)
	{
		if (hud->time_ >= 0)
		{
			hud->TimeUpdate();
		}
		else
		{
			enemy->unit_->AddCharacter({ 0,stage->TilePos(WIDTH / 2, 0).x,stage->spawnLine_ }, CHARA_TYPE::MOUSE, Puppet::DOWN);
			hud->time_ += ENEMY_SPAWN_INTERVAL;
		}
	}

	if (isReady)
		isTimeStart = true;

	if (Input::IsKeyDown(DIK_R))
	{
		stage->Initialize();
		hud->TutorialInitialize();
		player->unit_->Release();
		enemy->unit_->Release();
	}

	if (Input::IsKeyDown(DIK_E))
	{
		SceneManager* pSM = (SceneManager*)(FindObject("SceneManager"));
		pSM->ChangeScene(SCENE_ID::SCENE_ID_TITLE);
	}

}

void TutorialScene::Draw()
{ 
	isReady = true;
}

void TutorialScene::Release()
{
}
