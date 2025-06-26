#include "Mushroom.h"
#include "Engine/Model.h"
#include "PuppetFactory.h"

Mushroom::Mushroom(GameObject* parent)
{
}

void Mushroom::Initialize()
{
	LoadStatus(CHARA_TYPE::MUSHROOM);
	LoadParticle(CHARA_TYPE::MUSHROOM);
}

void Mushroom::Release()
{
}

void Mushroom::Attack()
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
