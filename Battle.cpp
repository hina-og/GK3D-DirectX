#include "Battle.h"
#include "Engine/Input.h"
#include "Engine/Time.h"
#include "Engine/Camera.h"
#include "SetUp.h"
#include "Engine/CsvReader.h"
#include "Engine/Text.h"
#include "GameOver.h"
#include "GameClear.h"

Battle::Battle(GameObject* parent)
	: GameObject(parent, "Battle")
{
}

void Battle::Initialize()
{
	XMFLOAT3 INIT_CAMERA_POSITION{ 0, 15, -4 };
	XMFLOAT3 INIT_CAMERA_TARGET{ 0, 0, -1 };

	Camera::SetPosition(INIT_CAMERA_POSITION);
	Camera::SetTarget(INIT_CAMERA_TARGET);

	CsvReader levelData;

	switch (SetUp::currentDifficulty)
	{
	case Difficulty::Easy:
		levelData.Load("GameData\\LevelEasy.csv");
		break;
	case Difficulty::Normal:
		levelData.Load("GameData\\LevelNormal.csv");
		break;
	case Difficulty::Hard:
		levelData.Load("GameData\\LevelHard.csv");
		break;
	case Difficulty::Despair:
		levelData.Load("GameData\\LevelDespair.csv");
		break;
	}
	for (int line = 0;line < levelData.GetLines();line++)
	{
		SpawnData sData;
		Puppet p;
		sData.type_ = p.CharacterType(levelData.GetString(line, 0));
		sData.time_ = levelData.GetFloat(line, 1);
		sData.line_ = levelData.GetInt(line, 2);

		spawnList_.push_back(sData);
	}
	spawnedNum = 0;

	stage = Instantiate<Stage>(this);
	player = Instantiate<Player>(this);
	enemy = Instantiate<Enemy>(this);
	
	
	//pot = Instantiate<Pot>(this);
	material = Instantiate<MaterialTable>(this);
	
	//pot->SetPosition(material->GetPosition().x, -450, 0);

	time = INIT_BATTLE_TIME;
	timeText.Initialize();
	isTimeStert = false;

	durability = MAX_DURABILITY;
	durabilityText.Initialize();

	getMaterialTime = 0.0;

	isReady = false;
	isBattleEnd = false;

	selectDir = Puppet::UP;
}

void Battle::Update()
{
	if (0 < durability && 0 < time)
	{
		for (int i = spawnedNum; i < spawnList_.size(); i++)
		{
			if (time < spawnList_[i].time_)
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

		if (Input::IsKeyDown(DIK_W))
		{
			selectDir = Puppet::UP;
		}
		else if (Input::IsKeyDown(DIK_A))
		{
			selectDir = Puppet::LEFT;
		}
		else if (Input::IsKeyDown(DIK_S))
		{
			selectDir = Puppet::DOWN;
		}
		else if (Input::IsKeyDown(DIK_D))
		{
			selectDir = Puppet::RIGHT;
		}

		//ƒ}ƒEƒX¶‚ð‰Ÿ‚µ‚Ä‚¢‚é‚Æ‚«
		if (Input::IsMouseButtonDown(LEFT_CLICK))
		{
			XMFLOAT2 mousePos = { (float)mouseX, (float)mouseY };
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

		enemy->unit_->PastLine(stage->endLine_, durability);

		if (1 <= getMaterialTime)
		{
			material->GetRandomMaterial();
			getMaterialTime--;
		}

		getMaterialTime += Time::GetDeltaTime();


		if (isTimeStert && time >= 0)
			time -= Time::GetDeltaTime();

		if (isReady)
			isTimeStert = true;
	}
	else if(!isBattleEnd)
	{
		if (time <= 0)
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
	timeText.Draw(1280,720,time + 1.0f);
	isReady = true;

	durabilityText.Draw(640, 980, durability);
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

