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
#include "PuppetFactory.h"

Battle::Battle(GameObject* parent)
	: GameObject(parent, "Battle")
{
}

void Battle::Initialize()
{
	hBGM_ = Audio::Load("Sounds\\BGM\\BGM.wav");
	assert(hBGM_ >= 0);

	Audio::Play(hBGM_);

	stage = Instantiate<Stage>(this);
	player = Instantiate<Player>(this);
	enemy = Instantiate<Enemy>(this);
	material = Instantiate<MaterialTable>(this);
	hud = Instantiate<HUD>(this);



	CsvReader levelData;
	
	std::string str = SetUp::currentDifficulty;
	levelData.Load(str);

	hud->time_=levelData.GetFloat(STAGE_DATA, TIME);
	hud->InitHP(levelData.GetInt(STAGE_DATA, HP));
	material->GiveMaterial(levelData.GetInt(STAGE_DATA, GIVE_MATERIAL_NUM));

	for (int line = 1;line < levelData.GetLines();line++)
	{
		SpawnData sData;
		Puppet p;
		sData.type_ = GetCharacterTypeFromName(levelData.GetString(line, SPAWN_NAME));
		sData.time_ = levelData.GetFloat(line, SPAWN_TIME);
		sData.line_ = levelData.GetInt(line, SPAWN_LINE);

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
	if (!Audio::IsPlaying(hBGM_))
	{
		Audio::Play(hBGM_);
	}

	XMFLOAT2 mousePos = { (float)mouseX, (float)mouseY };
	stage->SelectTilePosition(mousePos);


	//勝利条件
	bool endCondition[END_CONDITIONS::END] =
	{ 
		(spawnedNum >= spawnList_.size() && enemy->unit_->GetPuppetArray().size() <= 0),//敵がすべて出現して、全滅したか
		(hud->time_ <= 0),																//時間がなくなったか
		(hud->HP_ <= 0)																	//体力がなくなったか	
	};

	if (!endCondition[END_CONDITIONS::All_Kill] &&//敵がまだいる
		!endCondition[END_CONDITIONS::Time_Up]  &&//時間がまだある
		!endCondition[END_CONDITIONS::Dead])	  //体力がまだある
												  //このとき続ける
	{
		for (int i = spawnedNum; i < spawnList_.size(); i++)
		{
			if (hud->time_ < spawnList_[i].time_)
			{
				XMFLOAT3 pos = { (float)spawnList_[i].line_ - (WIDTH / 2 + 1),0,stage->spawnLine_ };
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

		//マウス左を押したらキャラ配置
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

		enemy->unit_->PastLine(stage->wallLine_, hud->HP_);

		if (GiveMaterialTime <= getMaterialTime)
		{
			material->GetRandomMaterial();
			getMaterialTime -= GiveMaterialTime;
		}

		getMaterialTime += Time::GetDeltaTime();


		if (isTimeStart && hud->time_ >= 0)
			hud->TimeUpdate();

		if (isReady)
			isTimeStart = true;
	}
	else if(!isBattleEnd)//バトルが終わったらシーン遷移する
	{
		if (hud->HP_ > 0)
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

