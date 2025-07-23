#include "Unit.h"
#include "Engine/Camera.h"
#include "Stage.h"
#include "PuppetFactory.h"

const float defaultShakePower{ 1.0f };//�ǂݍ��߂Ȃ������ꍇ�̃J�������h��鋭��
const float defaultShakeTime{ 100.0f };//�ǂݍ��߂Ȃ������ꍇ�̃J�������h��鎞�ԁi�~���b�j

Unit::Unit()
{
}

Unit::Unit(GameObject* parent)
	: GameObject(parent, "Unit")
{
}

void Unit::Initialize()
{
	//�J�������h��鋭���̓ǂݍ���
	shakePower_ = GetPrivateProfileInt("Unit", "camerashake_power", defaultShakePower, ".\\config.ini");
	//�J�������h��鎞�Ԃ̓ǂݍ���
	shakeTime_ = (float)GetPrivateProfileInt("Unit", "camerashake_time", defaultShakeTime, ".\\config.ini") / 1000;//�~���b����ϊ�
}

void Unit::Update()
{
	for (int p = 0; p < puppet_.size(); p++)
	{
		//�L����������ł�����z�񂩂�폜
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
	//�J������
	for (auto puppet : puppet_)
	{
		puppet->KillMe();
	}
	puppet_.clear();
}

void Unit::AddCharacter(XMFLOAT3 _pos, int _type, int _dir)
{
	//�L�����̔z��ɒǉ�
	puppet_.push_back(CreateInitPuppetByType(_type, this));

	//�ǉ������L�����̈ʒu�ƌ�����ݒ�
	puppet_.back()->SetPosition(_pos);
	puppet_.back()->SetDirection(_dir);
}

std::vector<Puppet*> Unit::GetPuppetArray()
{
	return puppet_;
}

void Unit::InvaderMove()
{
	for (auto puppet : puppet_)
	{
		//�U����ԂłȂ���Γ���
		if (!puppet->isAttack_)
			puppet->Move(puppet->dir_);

		if (!puppet->isAlive_)
			puppet->KillMe();
	}
}

void Unit::InRange(std::vector<Puppet*> _puppet)
{
	for(auto myPuppet : puppet_)
	{
		myPuppet->inRangeChara_.clear();//�U���͈͓��ɂ���L�����̔z������ꂢ�ɂ���
		myPuppet->isAttack_ = false;//�U�����Ă��Ȃ���Ԃɂ���
		std::vector<XMINT2> attackTiles = myPuppet->GetAttackTiles();//�U���͈͂̔z��

		for(auto enemy : _puppet)
		{
			//�Ώۂ����Ɏ���ł����玟�̃��[�v
			if (!enemy->isAlive_)
				continue;

			XMFLOAT3 enemyPos = enemy->GetPosition();//���̃��[�v�Œ��ׂĂ���G�̈ʒu

			for(auto range : attackTiles)
			{
				//�U���͈͂̒��S�̈ʒu
				XMFLOAT3 attackPos = {
					myPuppet->GetPosition().x + range.x,
					myPuppet->GetPosition().z + range.y,
					0
				};

				//�U���͈͂̈ʒu�ƓG�̈ʒu�̋���
				float distx = abs(attackPos.x - enemyPos.x),
					  disty = abs(attackPos.y - enemyPos.z);

				//�G���U���͈͓��ɂ���ꍇ
				if (distx < TILE_SIZE / 2 &&
					disty < TILE_SIZE / 2)
				{
					//�U�����Ă����Ԃɂ���
					myPuppet->isAttack_ = true;
					//�U���͈͓��ɂ���L�����̔z��ɓG��ǉ�
					myPuppet->inRangeChara_.push_back(enemy);
				}
			}

			if (myPuppet->isAttack_)
				break;
		}
	}
}

void Unit::PastLine(float _z, int& _durability)
{
	for (int p = 0; p < puppet_.size(); p++)
	{
		//�����Ă��ĕǂɓ��B�����ꍇ
		if (puppet_[p]->GetPosition().z < _z && !puppet_[p]->IsDead())
		{
			_durability -= puppet_[p]->GetPower();//�ϋv�l����U���͂̒l������
			puppet_[p]->KillMe();//���B�����L�����͏���
			puppet_.erase(puppet_.begin() + p);//�z�񂩂������

			Camera::StartShake(shakePower_, shakeTime_);//�J�����̐U���J�n
		}
	}
}

