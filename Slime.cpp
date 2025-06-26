#include "Slime.h"
#include "Engine/Model.h"
#include "PuppetFactory.h"

Slime::Slime(GameObject* parent)
{
}

void Slime::Initialize()
{
	LoadStatus(CHARA_TYPE::SLIME);
	LoadParticle(CHARA_TYPE::SLIME);
}

void Slime::Release()
{
}

void Slime::Attack()
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
