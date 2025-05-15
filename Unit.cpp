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
	// ���O�ɍU���͈͂��擾���ăL���b�V��
	for (int p = 0; p < puppet_.size(); p++)
	{
		puppet_[p]->isAttack_ = false;
		// �U���͈͂̃^�C�������O�Ɍv�Z
		const auto& attackTiles = puppet_[p]->GetAttackTiles(puppet_[p]->dir_);

		for (int e = 0; e < _puppet.size(); e++)
		{
			// �G�L�����N�^�[�������Ă��邩�ǂ������ŏ��Ƀ`�F�b�N
			if (!_puppet[e]->isAlive_)
				continue;

			// �G�L�����N�^�[�̈ʒu���L���b�V��
			const XMFLOAT3& enemyPos = _puppet[e]->GetPosition();

			// �U���͈͂̊e�^�C���ɂ���
			for (int rangeNum = 0; rangeNum < attackTiles.size(); rangeNum++)
			{
				XMFLOAT3 AttackPos = {
					puppet_[p]->GetPosition().x + attackTiles[rangeNum].x * 64,
					puppet_[p]->GetPosition().y + attackTiles[rangeNum].y * 64,
					0
				};

				// �����v�Z���ȑf��
				if (abs(AttackPos.x - enemyPos.x) < 64 &&
					abs(AttackPos.y - enemyPos.y) < 64)
				{
					puppet_[p]->isAttack_ = true;
					puppet_[p]->inRangeChara_.push_back(_puppet[e]);
					//break;  // 1�̂ł��͈͂ɓ���΁A�U���\�Ɣ��f
				}
			}

			// �͈͓��ɓG�L����������΁A���ʂȃ��[�v���I��
			if (puppet_[p]->isAttack_) break;
		}
	}
}
