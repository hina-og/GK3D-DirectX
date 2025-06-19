#include "Battle.h"
#include "Engine/Input.h"
#include "Engine/Time.h"
#include "Engine/Camera.h"
#include "SetUp.h"
#include "Engine/CsvReader.h"
#include "Engine/Text.h"
#include "GameOver.h"
#include "GameClear.h"
#include "Engine/Audio.h"
#include "File.h"

Battle::Battle(GameObject* parent)
	: GameObject(parent, "Battle")
{
}

void Battle::Initialize()
{
	hBGM_ = Audio::Load("Sounds\\BGM\\BGM5.wav");
	assert(hBGM_ >= 0);

	stage = Instantiate<Stage>(this);
	player = Instantiate<Player>(this);
	enemy = Instantiate<Enemy>(this);
	material = Instantiate<MaterialTable>(this);
	hud = Instantiate<HUD>(this);



	CsvReader levelData;
	
	std::string str = SetUp::currentDifficulty;
	levelData.Load(str);

	hud->time_=levelData.GetFloat(0, 0);
	hud->InitHP(levelData.GetInt(0, 1));
	material->GiveMaterial(levelData.GetInt(0, 2));

	for (int line = 1;line < levelData.GetLines();line++)
	{
		SpawnData sData;
		Puppet p;
		sData.type_ = p.CharacterType(levelData.GetString(line, 0));
		sData.time_ = levelData.GetFloat(line, 1);
		sData.line_ = levelData.GetInt(line, 2);

		spawnList_.push_back(sData);
	}
	spawnedNum = 0;

	isTimeStart = false;

	getMaterialTime = 0.0;

	isReady = false;
	isBattleEnd = false;

	selectDir = Puppet::UP;
}

void Battle::Update()
{
	XMFLOAT2 mousePos = { (float)mouseX, (float)mouseY };
	stage->SelectTilePosition(mousePos);

	if (0 < hud->HP_ && 0 < hud->time_)
	{
		for (int i = spawnedNum; i < spawnList_.size(); i++)
		{
			if (hud->time_ < spawnList_[i].time_)
			{
				XMFLOAT3 pos = { (float)spawnList_[i].line_ - (WIDTH / 2 + 1),0,stage->startLine_ };
				enemy->unit_->AddCharacter(pos, spawnList_[i].type_, Puppet::DOWN);
				spawnedNum++;
			}
			else
			{
				break;
			}
		}

		Input::GetMousePosition(mouseX, mouseY);

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

		//�}�E�X���������Ă���Ƃ�
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

		player->SetSelectTile(selectPos_);



		player->unit_->InRange(enemy->unit_->GetPuppetArray());
		enemy->unit_->InRange(player->unit_->GetPuppetArray());

		enemy->unit_->PastLine(stage->endLine_, hud->HP_);

		if (3 <= getMaterialTime)
		{
			material->GetRandomMaterial();
			getMaterialTime--;
		}

		getMaterialTime += Time::GetDeltaTime();


		if (isTimeStart && hud->time_ >= 0)
			hud->TimeUpdate();

		if (isReady)
			isTimeStart = true;
	}
	else if(!isBattleEnd)
	{
		if (hud->time_ <= 0)
		{
			Instantiate<GameClear>(this);
		}
		else
		{
			Instantiate<GameOver>(this);
		}
		isBattleEnd = true;
	}
}

void Battle::Draw()
{
	isReady = true;
}

void Battle::Release()
{
}

void Battle::SetSelectTile(XMFLOAT3 _selectPos)
{
	selectPos_ = _selectPos;
}

XMFLOAT3 Battle::GetSelectTile()
{
	return selectPos_;
}

