#include "Enemy.h"
#include "Engine/Input.h"

Enemy::Enemy(GameObject* parent)
	: GameObject(parent, "Enemy")
{
	//isAddEnemy_ = false;
	prevLeftClick = false;
}

void Enemy::Initialize()
{
	unit_ = Instantiate<Unit>(this);
}

void Enemy::Update()
{
	if ((Input::IsMouseButton(DIK_LEFT)))
	{
		//前のフレームでマウス左を押していないとき
		if (!prevLeftClick)
		{
			XMFLOAT3 pos = { 640 - 64 / 2, 0, 0 };
			unit_.AddCharacter(pos,ZOMBIE,Puppet::DIRECTION::DOWN);

			prevLeftClick = true;
		}
	}
	else
	{
		prevLeftClick = false;
	}
}

void Enemy::Draw()
{
}

void Enemy::Release()
{
}

