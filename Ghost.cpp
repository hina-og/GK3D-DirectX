#include "Ghost.h"
#include "Engine/Model.h"
#include "PuppetFactory.h"

Ghost::Ghost(GameObject* parent)
{
}

void Ghost::Initialize()
{
	LoadStatus(CHARA_TYPE::GHOST);
	LoadParticle(CHARA_TYPE::GHOST);
}

void Ghost::Release()
{
}

void Ghost::Attack()
{
	//攻撃範囲のエフェクト
	for (int rangeNum = 0;rangeNum < range_.size();rangeNum++)
	{
		particle_[rangeNum].position = rangePos_[rangeNum];
		VFX::Start(particle_[rangeNum]);
	}

	//HPを減らす
	for (int i = 0; i < inRangeChara_.size(); i++)
	{
		inRangeChara_[i]->ControlHP(-status_.power_);
		break;
	}
	attacked_ = true;
}
