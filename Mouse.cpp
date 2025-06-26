#include "Mouse.h"
#include "Engine/Model.h"

Mouse::Mouse(GameObject* parent)
{
}

void Mouse::Initialize()
{
	LoadStatus(CHARA_TYPE::MOUSE);
	LoadParticle(CHARA_TYPE::MOUSE);
}

void Mouse::Release()
{
}

void Mouse::Attack()
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
