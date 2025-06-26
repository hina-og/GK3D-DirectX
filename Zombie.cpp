#include "Zombie.h"
#include "Engine/Model.h"

Zombie::Zombie(GameObject* parent)
{
}

void Zombie::Initialize()
{
	LoadStatus(CHARA_TYPE::ZOMBIE);
	LoadParticle(CHARA_TYPE::ZOMBIE);
}

void Zombie::Release()
{
}

void Zombie::Attack()
{
	for (int rangeNum = 0;rangeNum < range_.size();rangeNum++)
	{
		particle_[rangeNum].position = rangePos_[rangeNum];
		VFX::Start(particle_[rangeNum]);
	}


	for (int i = 0; i < inRangeChara_.size(); i++)
	{
		inRangeChara_[i]->ControlHP(-status_.power_);
		break;
	}
	attacked_ = true;
}
