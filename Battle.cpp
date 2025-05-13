#include "Battle.h"
#include "Engine/Input.h"
#include "Time.h"
#include "Engine/Time.h"
#include "SetUp.h"
#include "Engine/CsvReader.h"

Battle::Battle(GameObject* parent)
	: GameObject(parent, "Battle")
{
	prevLeftClick = false;
	deltaTime = 0.0f;
}

void Battle::Initialize()
{
	CsvReader levelData;

	switch (SetUp::currentDifficulty)
	{
	case Difficulty::Easy:
		levelData.Load("GameData\\LevelEasy");
		break;
	case Difficulty::Normal:
		levelData.Load("GameData\\LevelNormal");
		break;
	case Difficulty::Hard:
		levelData.Load("GameData\\LevelHard");
		break;
	}

	stage = Instantiate<Stage>(this);
	player = Instantiate<Player>(this);
	enemy = Instantiate<Enemy>(this);
	//pot = Instantiate<Pot>(this);
	
	
	material = Instantiate<MaterialTable>(this);

	//Instantiate<Mouse>(this);

	time = INIT_BATTLE_TIME;
}

void Battle::Update()
{
	Input::GetMousePosition(mouseX, mouseY);


	XMFLOAT3 mousePos = { (float)mouseX - 32,(float)mouseY - 32,0 };
	selectPos_ = stage->SelectTile(mousePos);

	//マウス左を押しているとき
	if (Input::IsMouseButtonDown(LEFT_CLICK))
	{
		//前のフレームでマウス左を押していないとき
		if (!prevLeftClick && !stage->HasPlayer(selectPos_))
		{
			player->unit_->AddCharacter(selectPos_, 0, Puppet::UP);

			prevLeftClick = true;
		}
	}
	else
	{
		prevLeftClick = false;
	}

	player->SetSelectTile(selectPos_);

	player->unit_->InRange(enemy->unit_->GetPuppetArray());
	enemy->unit_->InRange(player->unit_->GetPuppetArray());

	time -= Time::GetDeltaTime();
}

void Battle::Draw()
{
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

