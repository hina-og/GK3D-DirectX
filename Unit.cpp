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
	// 事前に攻撃範囲を取得してキャッシュ
	for (int p = 0; p < puppet_.size(); p++)
	{
		puppet_[p]->isAttack_ = false;
		// 攻撃範囲のタイルを事前に計算
		const auto& attackTiles = puppet_[p]->GetAttackTiles(puppet_[p]->dir_);

		for (int e = 0; e < _puppet.size(); e++)
		{
			// 敵キャラクターが生きているかどうかを最初にチェック
			if (!_puppet[e]->isAlive_)
				continue;

			// 敵キャラクターの位置をキャッシュ
			const XMFLOAT3& enemyPos = _puppet[e]->GetPosition();

			// 攻撃範囲の各タイルについて
			for (int rangeNum = 0; rangeNum < attackTiles.size(); rangeNum++)
			{
				XMFLOAT3 AttackPos = {
					puppet_[p]->GetPosition().x + attackTiles[rangeNum].x * 64,
					puppet_[p]->GetPosition().y + attackTiles[rangeNum].y * 64,
					0
				};

				// 距離計算を簡素化
				if (abs(AttackPos.x - enemyPos.x) < 64 &&
					abs(AttackPos.y - enemyPos.y) < 64)
				{
					puppet_[p]->isAttack_ = true;
					puppet_[p]->inRangeChara_.push_back(_puppet[e]);
					//break;  // 1体でも範囲に入れば、攻撃可能と判断
				}
			}

			// 範囲内に敵キャラがいれば、無駄なループを終了
			if (puppet_[p]->isAttack_) break;
		}
	}
}
