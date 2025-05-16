#include "Unit.h"

Unit::Unit()
{
}

Unit::Unit(GameObject* parent)
	: GameObject(parent, "Unit")
{
}

void Unit::Initialize()
{
}

void Unit::Update()
{
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
		puppet_[p]->Move(puppet_[p]->dir_);
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
