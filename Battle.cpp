#include "Battle.h"
#include "Engine/Input.h"

Battle::Battle(GameObject* parent)
	: GameObject(parent, "Battle")
{
	prevLeftClick = false;
}

void Battle::Initialize()
{
	
	stage = Instantiate<Stage>(this);
	player = Instantiate<Player>(this);
	enemy = Instantiate<Enemy>(this);
	//pot = Instantiate<Pot>(this);
	
	
	material = Instantiate<MaterialTable>(this);

	Instantiate<Mouse>(this);
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
			player->unit_.AddCharacter(selectPos_, 0, Puppet::UP);

			prevLeftClick = true;
		}
	}
	else
	{
		prevLeftClick = false;
	}

	player->SetSelectTile(selectPos_);

	player->unit_.InRange(enemy->unit_.GetPuppetArray());
	enemy->unit_.InRange(player->unit_.GetPuppetArray());
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

