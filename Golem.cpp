#include "Golem.h"
#include "Engine/Model.h"

Golem::Golem(GameObject* parent)
{
}

void Golem::Initialize()
{
	LoadStatus(GOLEM);
}

void Golem::Release()
{
}

void Golem::Attack()
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
