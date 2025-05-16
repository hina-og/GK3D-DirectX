#include "Battle.h"
#include "Engine/Input.h"
#include "Time.h"
#include "Engine/Time.h"
#include "SetUp.h"
#include "Engine/CsvReader.h"
#include "Engine/Text.h"

Battle::Battle(GameObject* parent)
	: GameObject(parent, "Battle")
{
}

void Battle::Initialize()
{
	CsvReader levelData;

	SetUp::currentDifficulty = Difficulty::Easy;

	switch (SetUp::currentDifficulty)
	{
	case Difficulty::Easy:
		levelData.Load("GameData\\LevelDie.csv");
		break;
	case Difficulty::Normal:
		levelData.Load("GameData\\LevelNormal.csv");
		break;
	case Difficulty::Hard:
		levelData.Load("GameData\\LevelHard.csv");
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

	//Instantiate<Mouse>(this);

	time = INIT_BATTLE_TIME;
	timeText.Initialize();
	isTimeStert = false;

	isReady = false;
}

void Battle::Update()
{
	for (int i = spawnedNum;i < spawnList_.size();i++)
	{
		if (time < spawnList_[i].time_)
		{
			XMFLOAT3 pos = { (float)spawnList_[i].line_ - (WIDTH / 2 + 1),0,HEIGHT / 2 + 1};
			enemy->unit_->AddCharacter(pos, spawnList_[i].type_, Puppet::DOWN);
			spawnedNum++;
		}
		else
		{
			break;
		}
	}
	
	Input::GetMousePosition(mouseX, mouseY);



	//ƒ}ƒEƒX¶‚ð‰Ÿ‚µ‚Ä‚¢‚é‚Æ‚«
	if (Input::IsMouseButtonDown(LEFT_CLICK))
	{
		XMFLOAT2 mousePos = { (float)mouseX, (float)mouseY };
		XMFLOAT2 tileNum = { -1, -1 };
		XMFLOAT3 tilePos = {};

		bool hit = stage->SelectTile(mousePos, tileNum, tilePos);
		if (hit && !stage->HasPlayer(tileNum)) {
			selectPos_ = tilePos;
			player->unit_->AddCharacter(selectPos_, 0, Puppet::UP);
		}


		/*XMFLOAT2 mousePos = { (float)mouseX,(float)mouseY };
		selectPos_ = stage->SelectTilePosition(mousePos);
		if (!stage->HasPlayer(stage->SelectTileNumber(mousePos)))
			player->unit_->AddCharacter(selectPos_, 0, Puppet::UP);*/
	}

	player->SetSelectTile(selectPos_);

	//player->unit_->InvaderMove();
	enemy->unit_->InvaderMove();

	player->unit_->InRange(enemy->unit_->GetPuppetArray());
	enemy->unit_->InRange(player->unit_->GetPuppetArray());
	

	if(isTimeStert && time >= 0)
		time -= Time::GetDeltaTime();

	if (isReady)
		isTimeStert = true;

}

void Battle::Draw()
{
	timeText.Draw(1000,0,time + 1.0f);
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

