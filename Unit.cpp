#include "Unit.h"
#include "Engine/Camera.h"

Unit::Unit()
{
}

Unit::Unit(GameObject* parent)
	: GameObject(parent, "Unit")
{
}

void Unit::Initialize()
{
	shakePower_ = GetPrivateProfileInt("Unit", "camerashake_power", 1, ".\\config.ini");
	shakeTime_ = (float)GetPrivateProfileInt("Unit", "camerashake_time", 100, ".\\config.ini") / 1000;//ƒ~ƒŠ•b‚©‚ç•ÏŠ·
}

void Unit::Update()
{
	for (int p = 0; p < puppet_.size(); p++)
	{
		if (puppet_[p]->IsDead())
		{
			puppet_.erase(puppet_.begin() + p);
		}
	}
}

void Unit::Draw()
{
}

void Unit::Release()
{
}

void Unit::AddCharacter(XMFLOAT3 _pos, int _type, Puppet::DIRECTION _dir)
{
	switch (_type)
	{
	case MOUSE:
		puppet_.push_back(Instantiate<Mouse>(this));
		break;
	case ZOMBIE:
		puppet_.push_back(Instantiate<Zombie>(this));
		break;
	case SLIME:
		puppet_.push_back(Instantiate<Slime>(this));
		break;
	case GOLEM:
		puppet_.push_back(Instantiate<Golem>(this));
		break;
	case GHOST:
		puppet_.push_back(Instantiate<Ghost>(this));
		break;
	default:
		break;
	}
	puppet_.back()->SetPosition(_pos);
	puppet_.back()->SetDirection(_dir);
}

void Unit::AddCharacter(XMFLOAT3 _pos, int _type, int _dir)
{
	switch (_type)
	{
	case MOUSE:
		puppet_.push_back(Instantiate<Mouse>(this));
		break;
	case ZOMBIE:
		puppet_.push_back(Instantiate<Zombie>(this));
		break;
	case MUSHROOM:
		puppet_.push_back(Instantiate<Mushroom>(this));
		break;
	case SLIME:
		puppet_.push_back(Instantiate<Slime>(this));
		break;
	case GOLEM:
		puppet_.push_back(Instantiate<Golem>(this));
		break;
	case GHOST:
		puppet_.push_back(Instantiate<Ghost>(this));
		break;
	default:
		break;
	}
	puppet_.back()->SetPosition(_pos);
	puppet_.back()->SetDirection(_dir);
}

std::vector<Puppet*> Unit::GetPuppetArray()
{
	return puppet_;
}

void Unit::InvaderMove()
{
	for (int p = 0; p < puppet_.size(); p++)
	{
		if(!puppet_[p]->isAttack_)
			puppet_[p]->Move(puppet_[p]->dir_);

		if (!puppet_[p]->isAlive_)
			puppet_[p]->KillMe();
	}
}

void Unit::InRange(std::vector<Puppet*> _puppet)
{
	for (int my = 0; my < puppet_.size(); my++)
	{
		puppet_[my]->inRangeChara_.clear();
		puppet_[my]->isAttack_ = false;
		std::vector<Puppet::Pos> attackTiles = puppet_[my]->GetAttackTiles(puppet_[my]->dir_);

		for (int enemy = 0; enemy < _puppet.size(); enemy++)
		{
			if (!_puppet[enemy]->isAlive_)
				continue;

			XMFLOAT3 enemyPos = _puppet[enemy]->GetPosition();

			for (int rangeNum = 0; rangeNum < attackTiles.size(); rangeNum++)
			{
				XMFLOAT3 attackPos = {
					puppet_[my]->GetPosition().x + attackTiles[rangeNum].x,
					puppet_[my]->GetPosition().z + attackTiles[rangeNum].y,
					0
				};

				float x = abs(attackPos.x - enemyPos.x), y = abs(attackPos.y - enemyPos.z);
				if (x < 1.0 &&
					y < 1.0)
				{
					puppet_[my]->isAttack_ = true;
					puppet_[my]->inRangeChara_.push_back(_puppet[enemy]);
				}
			}

			if (puppet_[my]->isAttack_)
				break;
		}
	}
}

void Unit::PastLine(float _z, int& _durability)
{
	for (int p = 0; p < puppet_.size(); p++)
	{
		if (puppet_[p]->GetPosition().z < _z && !puppet_[p]->IsDead())
		{
			_durability -= puppet_[p]->GetPower();
			puppet_[p]->KillMe();
			puppet_.erase(puppet_.begin() + p);

			Camera::StartShake(shakePower_, shakeTime_);
		}
	}
}
